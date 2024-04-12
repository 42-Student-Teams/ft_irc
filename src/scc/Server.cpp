/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 23:13:14 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/12 11:16:16 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scc/Server.hpp"

/*-----------------------------CONSTRUT && DESTRUCT--------------------------*/

Server::Server(const std::string &port, const std::string &pass) 
    : _host("127.0.0.1"), _port(port), _pass(pass)
{
    _serverStatus = 1;
    _socketFD = createSocket();
   _parser = new Parser(this);
}

Server::~Server() 
{
    delete _parser;

    for (std::size_t i = 0; i < _channels.size(); i++)
        delete _channels[i];

    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        delete it->second;
}


/*-----------------------------MAIN LOOP SERVER--------------------------*/

/*Le server s ouvre avec le port initialisé = OK
il mentionne que le serveur et ON*/
void Server::run()
{
    // Ajoute le serveur au tableau de poll
    pollfd srv = {_socketFD, POLLIN, 0};
    _pfds.push_back(srv);

    // Obtien les informations d'adresse locale pour récupérer le numéro de port
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    if (getsockname(_socketFD, (struct sockaddr*)&addr, &addr_len) == -1)
        throw std::runtime_error("ERROR : socket connection issue!");
    
    // Converti l'ordre des octets du réseau en l'ordre des octets de l'hôte
    int port = ntohs(addr.sin_port); 

    // Affiche le message incluant le numéro de port
    std::string message = "Server is listening on port " + std::to_string(port) + "...";
    serverON(message.c_str());

    // Exécution de la boucle principale en attendant les connexions
    while (_serverStatus)
    {
        if (poll(_pfds.data(), _pfds.size(), -1) < 0)
            throw std::runtime_error("ERROR: POLLING fd issue!");

        // Un ou plusieurs descripteurs sont prêts à être lus => les traiter

        for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
        {
            if (it->revents == 0)
                continue;

            if ((it->revents & POLLHUP) == POLLHUP)
            {
                handleClientDisconnect(it->fd);
                break;
            }

            if ((it->revents & POLLIN) == POLLIN)
            {
                if (it->fd == _socketFD)
                {
                    handleClientConnection();
                    break;
                }
                handleClientMessage(it->fd);
            }
        }
    }
}

/*-----------------------------ACCESSORS--------------------------*/
std::string Server::getPassword() const 
{
    return _pass;
}


Client* Server::getClient(const std::string& nickname)
{
    std::map<int, Client *>::iterator it_b = _clients.begin();
    std::map<int, Client *>::iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        if (!nickname.compare(it_b->second->getNickname()))
            return it_b->second;

        it_b++;
    }

    return nullptr;
}

//OK MAIS STOCKER DIFF 
Channel* Server::getChannel(const std::string& name)
{
    for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (!name.compare((*it)->getName()))
            return (*it);
    }

    return nullptr;
}

std::vector<Channel*> Server::getAllChannels() const 
{
    return _channels;
}

int Server::getClientCount() const {
    return _clients.size();
}


/*-----------------------------MANAGE NETWORK CONNECTIONS TO THE SERVER--------------------------*/

/*Le server accepte les connections : OK 
et il mentionne que le localhost et bien connecté ou pas
Manage Connections
 Il accepte la connexion du client, récupère son nom d'hôte, crée un objet Client pour représenter le client connecté, envoie un message de bienvenue au client et notifie le serveur de la connexion du client.*/
void Server::handleClientConnection()
{
    int fd;
    sockaddr_in addr = {};
    socklen_t size = sizeof(addr);

    fd = accept(_socketFD, (sockaddr *)&addr, &size);
    if (fd < 0)
        throw std::runtime_error("ERROR: Client connection failed!");

    pollfd pfd = {fd, POLLIN, 0};
    _pfds.push_back(pfd);

    char hostname[NI_MAXHOST];
    int res = getnameinfo((struct sockaddr *)&addr, size, hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
    if (res != 0)
        throw std::runtime_error("ERROR: hosting_name setup failed!");

    Client* client = new Client(fd, ntohs(addr.sin_port), hostname);
    _clients.insert(std::make_pair(fd, client));

    int numClients = _clients.size();
    std::string connectMessage = client->getHostname() + ":" + std::to_string(client->getPort()) + " has connected. There are now " + std::to_string(numClients) + " clients connected.";

    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        sendMessage(it->first, connectMessage);
    }

    std::cout << connectMessage << std::endl;
}


void Server::handleClientDisconnect(int fd)
{
    try
    {
        // Cherche le client et supprime s'il quitte
        Client* client = _clients.at(fd); 
        client->leave();

        // Préparer le message de déconnexion
        std::string disconnectMessage = client->getHostname() + ":" + std::to_string(client->getPort()) + " Successfully disconnected! ";

        // Suppression du client de la liste
        _clients.erase(fd);

        // Informer tous les autres clients
        int numClients = _clients.size(); // Obtenir le nombre de clients restants
        disconnectMessage += "There are now " + std::to_string(numClients) + " clients connected..\n";

        for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
            sendMessage(it->first, disconnectMessage); // Envoyer le message à chaque client actif
        }

        // Suppression du descripteur de fichier du poll
        for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); ) {
            if (it->fd == fd) {
                it = _pfds.erase(it);
            } else {
                ++it;
            }
        }

        close(fd); // Fermer le descripteur de fichier
        delete client; // Libérer la mémoire du client

        // Journalisation de la déconnexion
        std::cout << disconnectMessage << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "ERROR: The connection has been forcibly terminated! " << e.what() << std::endl;
    }
}

void Server::handleClientMessage(int fd)
{
    try
    {
        Client *client = _clients.at(fd);
        std::string message = this->recvMsgFrom(fd);

        if (client->registrationCheck()) {
            // Si le client est enregistré, traiter n'importe quelle commande
            _parser->processMessage(client, message);
        } else {
            // Si le client n'est pas enregistré, limiter les commandes à NICK, PASS, et USER
            if (message.rfind("NICK", 0) == 0 || message.rfind("PASS", 0) == 0 || message.rfind("USER", 0) == 0) {
                _parser->processMessage(client, message);
            } else {
                // Envoyer un message d'erreur si le client tente d'autres commandes
                sendMessage(fd, "Vous devez vous enregistrer (NICK/PASS/USER) pour accéder aux cmds serveur\n");
            }
        }
    }
    catch (const std::out_of_range&) 
    {
        // Gestion spécifique pour les cas où le client n'est pas trouvé dans le map
        std::cout << "Error: Client not found for file descriptor " << fd << std::endl;
    }
    catch (const std::exception& e) 
    {
        // Autres erreurs générales
        std::cout << "Error: message/command not received! " << e.what() << std::endl;
    }
}

void Server::sendMessage(int fd, const std::string& message)
{
    // Vérifie si le descripteur de fichier est valide
    if (fd < 0)
    {
        std::cerr << "Invalid file descriptor." << std::endl;
        return;
    }

    // Envoie le message au client
    ssize_t sentBytes = send(fd, message.c_str(), message.length(), 0);

    // Vérifie s'il y a eu des erreurs lors de l'envoi du message
    if (sentBytes == -1)
    {
        std::cerr << "Error sending message to client." << std::endl;
        // Vous pouvez ajouter ici des actions supplémentaires en cas d'erreur
    }
}

std::string Server::recvMsgFrom(int fd)
{
    std::string message;
    
    char buffer[100];
    bzero(buffer, 100);

    while (!strstr(buffer, "\n"))
    {
        bzero(buffer, 100);

        if ((recv(fd, buffer, 100, 0) < 0) and (errno != EWOULDBLOCK))
            throw std::runtime_error("Error while reading buffer from a client!");

        message.append(buffer);
    }

    return message;
}

int Server::createSocket()
{
    // ouverture d'un socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
        throw std::runtime_error("ERROR : socket failed to connect!");

    // attachement forcé du socket au port en le rendant réutilisable
    int optval = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
        throw std::runtime_error("ERROR: socket port parameters failed!");

    // rendre le socket non bloquant
    if (fcntl(sock_fd, F_SETFL, O_NONBLOCK))
        throw std::runtime_error("ERROR : making the socket non-blocking!");

    // collecte des données nécessaires pour le bind
    struct sockaddr_in serv_addr = {};
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(_port.c_str()));

    // liaison du socket à une adresse IP sur le port sélectionné
    if (bind(sock_fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        throw std::runtime_error("ERROR : linking socket ip/port failed");

    // écoute des requêtes
    if (listen(sock_fd, MAX_USER) < 0)
        throw std::runtime_error("ERROR : listen socket request failed!");

    return sock_fd;
}

//under construction, stock the new Channel in the std::vector<Channel *>  _channels
Channel* Server::createChannel(const std::string& name, const std::string& key, Client* client)
{
    Channel *channel = new Channel(name, key, client);
    _channels.push_back(channel);

    return channel;
}