/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Probook <Probook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 23:13:14 by inaranjo          #+#    #+#             */
/*   Updated: 2024/03/29 16:36:09 by Probook          ###   ########.fr       */
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

Channel* Server::getChannel(const std::string& name)
{
    for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (!name.compare((*it)->getName()))
            return (*it);
    }

    return nullptr;
}

std::vector<Channel*> getAllChannels() {
    return _channels;
}

/*-----------------------------MANAGE NETWORK CONNECTIONS TO THE SERVER--------------------------*/

/*Le server accepte les connections : OK 
et il mentionne que le localhost et bien connecté ou pas
Manage Connections*/
void Server::handleClientConnection()
{
    // Accepter une connexion
    int         fd;
    sockaddr_in addr = {};
    socklen_t   size = sizeof(addr);

    fd = accept(_socketFD, (sockaddr *) &addr, &size);
    if (fd < 0)
        throw std::runtime_error("ERROR : Client connection failed!");

    // Ajouter le descripteur du client dans le poll
    pollfd  pfd = {fd, POLLIN, 0};
    _pfds.push_back(pfd);

    // Obtenir le nom d'hôte à partir de l'adresse du client
    char hostname[NI_MAXHOST];
    int res = getnameinfo((struct sockaddr *) &addr, sizeof(addr), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
    if (res != 0)
        throw std::runtime_error("ERROR : hosting_name setup failed!");

    // Crée et enregistre un nouveau client
    Client* client = new Client(fd, ntohs(addr.sin_port), hostname);
    _clients.insert(std::make_pair(fd, client));

    //message de connexion pour le serveur
    std::string message = client->getHostname() + ":" + std::to_string(client->getPort()) + " has connected.";
    serverON(message);
}

/*Manage disconnection*/
void Server::handleClientDisconnect(int fd)
{
    try
    {
        //cherche le client et supprime if leave
        Client* client = _clients.at(fd); 
        client->leave();

        // Journalisation de la déconnexion
        std::string message = client->getHostname() + ":" + std::to_string(client->getPort()) + " Successfully connected!"; 

        _clients.erase(fd);

        // Suppression du descripteur de fichier client du poll
        std::vector<pollfd>::iterator it;
        for (it = _pfds.begin(); it != _pfds.end(); ++it)
        {
            if (it->fd == fd) 
            {
                _pfds.erase(it);
                break;
            }
        }
        close(fd);
        delete client;
    }
    catch (const std::exception &e)
    {
        std::cout << "ERROR : The connection has been forcibly terminated!" << e.what() << std::endl;
    }
}

/*ATTENTION: parsing nécessaire pour la gestion des messages intra client,server,channel
Manage message/cmd reception*/
void Server::handleClientMessage(int fd)
{
    try
    {
        Client *client = _clients.at(fd);
        std::string message = this->recvMsgFrom(fd);
        
        _parser->processMessage(client, message);
    }
    catch (const std::exception& e) 
    {
        std::cout << "Error : message/command not receve ! " << e.what() << std::endl;
    }
}

std::string Server::recvMsgFrom(int fd)
{
    std::string message;
    
    char buffer[100];
    bzero(buffer, 100);

    for (;;)
    {
        if (strstr(buffer, "\n"))
            break;
        bzero(buffer, 100);
        if ((recv(fd, buffer, 100, 0) < 0) and (errno != EWOULDBLOCK))
            throw std::runtime_error("ERROR : Buffer msg issue!");
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