/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 23:13:14 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/18 18:18:56 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scc/Server.hpp"

/*-----------------------------CONSTRUT && DESTRUCT--------------------------*/

Server::Server(const std::string &port, const std::string &pass)
    : _host("127.0.0.1"), _port(port), _pass(pass)
{
    _serverStatus = 1;
    //_socketFD = createSocket();
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

//------------------old version---------//
// void Server::run()
// {
//     pollfd srv = {_socketFD, POLLIN, 0};
//     _pfds.push_back(srv);

//     // Obtien les informations d'adresse locale pour récupérer le numéro de port
//     struct sockaddr_in addr;
//     socklen_t addr_len = sizeof(addr);
//     if (getsockname(_socketFD, (struct sockaddr*)&addr, &addr_len) == -1)
//         throw std::runtime_error("ERROR : socket connection issue!");

//     // Converti l'ordre des octets du réseau en l'ordre des octets de l'hôte
//     int port = ntohs(addr.sin_port);

//     // Affiche le message incluant le numéro de port
//     std::string message = "Server is listening on port " + std::to_string(port) + "...";
//     serverON(message.c_str());

//     while (_serverStatus)
//     {
//         if (poll(_pfds.data(), _pfds.size(), -1) < 0)
//             throw std::runtime_error("ERROR: POLLING fd issue!");

//         // Un ou plusieurs descripteurs sont prêts à être lus => les traiter

//         for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
//         {
//             if (it->revents == 0)
//                 continue;

//             if ((it->revents & POLLHUP) == POLLHUP)
//             {
//                 handleClientDisconnect(it->fd);
//                 break;
//             }

//             if ((it->revents & POLLIN) == POLLIN)
//             {
//                 if (it->fd == _socketFD)
//                 {
//                     handleClientConnection();
//                     break;
//                 }
//                 handleClientMessage(it->fd);
//             }
//         }
//     }
// }

// new version
void Server::run()
{
    this->createSocket();

    std::cout << "Waiting to accept a connection...\n";
    while (_serverStatus)
    {
        if (poll(_pfds.data(), _pfds.size(), -1) < 0)
            throw std::runtime_error("ERROR: POLLING fd issue!");

        for (size_t i = 0; i < _pfds.size(); i++)
        {
            if (_pfds[i].revents & POLLIN)
            {
                if (_pfds[i].fd == _socketFD)
                    this->handleClientConnection();
                else
                    this->handleClientMessage(_pfds[i].fd);
            }
        }
    }
}

/*-----------------------------GETTERS--------------------------*/
std::string Server::getPassword() const
{
    return _pass;
}

// -----old version-----//
Client *Server::getClientStr(const std::string &nickname)
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

// NEW VERSION
Client *Server::getClient(int fd)
{
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->first == fd)   // Vérifie si le descripteur de fichier correspond
            return it->second; // Retourne le pointeur vers le client
    }
    return nullptr;
}

Channel *Server::getChannel(const std::string &name)
{
    for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (!name.compare((*it)->getName()))
            return (*it);
    }

    return nullptr;
}

std::vector<Channel *> Server::getAllChannels() const
{
    return _channels;
}

int Server::getClientCount() const
{
    return _clients.size();
}

/*-------------------------------------SETTERS---------------------------------------------------*/

void Server::setFD(int fd) { this->_socketFD = fd; }

/*-----------------------------MANAGE NETWORK CONNECTIONS TO THE SERVER--------------------------*/

// void Server::handleClientConnection()
// {
//     int fd;
//     sockaddr_in addr = {};
//     socklen_t size = sizeof(addr);

//     fd = accept(_socketFD, (sockaddr *)&addr, &size);
//     if (fd < 0)
//         throw std::runtime_error("ERROR: Client connection failed!");

//     pollfd pfd = {fd, POLLIN, 0};
//     _pfds.push_back(pfd);

//     char hostname[NI_MAXHOST];
//     int res = getnameinfo((struct sockaddr *)&addr, size, hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
//     if (res != 0)
//         throw std::runtime_error("ERROR: hosting_name setup failed!");

//     Client* client = new Client(fd, ntohs(addr.sin_port), hostname);
//     _clients.insert(std::make_pair(fd, client));

//     int numClients = _clients.size();
//     std::string connectMessage = BLUE + client->getHostname() + RESET + ":\n" +
//                              VIOLET + std::to_string(client->getPort()) + RESET +
//                              " has connected. There are now " +
//                              YELLOW + std::to_string(numClients) + RESET +
//                              " user connected::press enter";

//     for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
//         sendMessage(it->first, connectMessage);
//     }

//     std::cout << connectMessage << std::endl;
// }

// new version
void Server::handleClientConnection()
{
    Client *user = NULL;
    memset(&adduser, 0, sizeof(adduser)); // link to struct sockaddr
    socklen_t len = sizeof(adduser);
    int fd = accept(_socketFD, (sockaddr *)&(adduser), &len);
    if (fd == -1)
    {
        std::cout << "accept() failed" << std::endl;
        return;
    }
    if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cout << "fcntl() failed" << std::endl;
        return;
    }
    new_cli.fd = fd;
    new_cli.events = POLLIN;
    new_cli.revents = 0;
    user->setFD(fd);
    user->setIpAdd(inet_ntoa((adduser.sin_addr)));
    _clients[fd] = user;
    _pfds.push_back(new_cli);
    std::cout << GREEN << "Client <" << fd << "> Connected" << RESET << std::endl;
}

void Server::handleClientDisconnect(int fd)
{
    if (fd == 0)
    {
        std::cout << "fd -> 0 ignored from handleClientDisconnect" << std::endl; // debug
        return;
    }
    std::cout << RED << "Client <" << fd << "> Disconnected" << RESET << std::endl;

    // remove the user from the channels they are in
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end();)
    {
        if (it->first == fd)
        {
            delete it->second;
            _clients.erase(it++);
        }
        else
        {
            ++it;
        }
    }

    // remove fd from the client in the pollFD (vector)
    for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end();)
    {
        if (it->fd == fd)
        {
            it = _pfds.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // close the FD
    close(fd);
}

void Server::sendMessage(int fd, const std::string &message)
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

//---------------------OLD VERSION------------------//
// void Server::handleClientMessage(int fd)
// {
//     // ne traite pas stdin fd ---> 0
//     if (fd == 0) {
//         std::cout << "fd -> 0 ignored from handleClientMessage " << std::endl; //debug
//         return;
//     }

//     // Vérifier si le client existe dans le conteneur _clients
//     if (_clients.find(fd) != _clients.end())
//     {
//         try
//         {
//             Client *client = _clients.at(fd);
//             std::string message = this->recvMsgFrom(fd);

//             if (client->registrationCheck()) {
//                 // Si le client est enregistré, traiter n'importe quelle commande
//                 _parser->excInput(client, message);
//             } else {
//                 // Si le client n'est pas enregistré, limiter les commandes à NICK, PASS, USER et QUIT
//                 if (message.rfind("NICK", 0) == 0 || message.rfind("PASS", 0) == 0 || message.rfind("USER", 0) == 0 || message.rfind("QUIT", 0) == 0) {
//                     _parser->excInput(client, message);
//                 } else {
//                     // Envoyer un message d'erreur si le client tente d'autres commandes
//                     sendMessage(fd, "Vous devez vous enregistrer (NICK/PASS/USER) pour accéder aux commandes serveur\n");
//                 }
//             }
//         }
//         catch (const std::out_of_range&)
//         {
//             // Gestion spécifique pour les cas où le client n'est pas trouvé dans le map
//             std::cout << "Error: Client not found for file descriptor " << fd << std::endl;
//         }
//         catch (const std::exception& e)
//         {
//             // Autres erreurs générales
//             std::cout << "Error: message/command not received! " << e.what() << std::endl;
//         }
//     }
//     // else
//     // {
//     //     std::cout << "Error: Client with file descriptor " << fd << " not found." << std::endl;
//     // }
// }

// NEW VERSION
void Server::handleClientMessage(int fd)
{
    char buffer[1024];
    bzero(buffer, 1024);

    Client *user = getClient(fd);
    Parser *bigPars = NULL;
    ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0)
    {
        this->handleClientDisconnect(fd);
    }
    else
    {
        bigPars->excInput(user, buffer);
    }
}

//----------------OLD VERSION
// int Server::createSocket()
// {
//     // ouverture d'un socket
//     int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sock_fd < 0)
//         throw std::runtime_error("ERROR : socket failed to connect!");

//     // attachement forcé du socket au port en le rendant réutilisable
//     int optval = 1;
//     if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
//         throw std::runtime_error("ERROR: socket port parameters failed!");

//     // rendre le socket non bloquant
//     if (fcntl(sock_fd, F_SETFL, O_NONBLOCK))
//         throw std::runtime_error("ERROR : making the socket non-blocking!");

//     // collecte des données nécessaires pour le bind
//     struct sockaddr_in serv_addr = {};
//     bzero((char*)&serv_addr, sizeof(serv_addr));
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_addr.s_addr = INADDR_ANY;
//     serv_addr.sin_port = htons(atoi(_port.c_str()));

//     // liaison du socket à une adresse IP sur le port sélectionné
//     if (bind(sock_fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
//         throw std::runtime_error("ERROR : linking socket ip/port failed");

//     // écoute des requêtes
//     if (listen(sock_fd, MAX_USER) < 0)
//         throw std::runtime_error("ERROR : listen socket request failed!");

//     return sock_fd;
// }

// NEW VERSION
void Server::createSocket()
{
    int optval = 1;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(_port.c_str()));
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd == -1)
        throw(std::runtime_error("faild to create socket"));
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
    if (fcntl(sock_fd, F_SETFL, O_NONBLOCK) == -1)
        throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
    if (bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        throw(std::runtime_error("faild to bind socket"));
    if (listen(sock_fd, SOMAXCONN) == -1)
        throw(std::runtime_error("listen() faild"));

    new_cli.fd = sock_fd;
    new_cli.events = POLLIN;
    new_cli.revents = 0;
    _pfds.push_back(new_cli);
}

// under construction, stock the new Channel in the std::vector<Channel *>  _channels
Channel *Server::createChannel(const std::string &name, const std::string &key, Client *client)
{
    Channel *channel = new Channel(name, key, client);
    _channels.push_back(channel);

    return channel;
}