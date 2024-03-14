#include "Server.hpp"


Server::Server(const int &port, const std::string &password): _port(port), _password(password)
{
    _socketFD = -1;
    _fds.clear();
}

Server::~Server()
{
    if (_socketFD != -1) 
        close(_socketFD);
}

void Server::initServer()
{
    _socketFD= socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFD < 0) 
    {
        std::cerr << "ERROR :Socket could not be created" << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(_port);

    if (bind(_socketFD, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cerr << "ERROR : Bind could not process." << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(_socketFD, 5) < 0) 
    {
        std::cerr << "Erreur lors de l'écoute." << std::endl;
        exit(EXIT_FAILURE);
    }
    fcntl(_socketFD, F_SETFL, O_NONBLOCK); // socket non bloquant

    std::cout << "Serveur initialisé sur le port " << _port << std::endl;
}
 
void Server::run()
{
    initServer();

    /* la struct pollfd va permetre de surveiller les events des fd*/
    struct pollfd check;
    check.fd = _socketFD;
    check.events = POLLIN;// Ici, POLLIN indique qu'on surveille les données prêtes à être lues

    _fds.push_back(check);// Ajout de la structure check dans le vecteur _fds

    while (true) 
    {
        int ret = poll(_fds.data(), _fds.size(), -1);

        if (ret < 0) 
        {
            std::cerr << "Error poll." << std::endl;
            break;
        }

       for (size_t i = 0; i < _fds.size(); i++) 
       {
            if (_fds[i].revents & POLLIN) 
            {
                if (_fds[i].fd == _socketFD)
                    acceptNewConnection();
                else 
                    handleClient(_fds[i].fd);
            }
       }
    }
}

void Server::acceptNewConnection()
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSock = accept(_socketFD, (struct sockaddr *)&clientAddr, &clientAddrLen);

    if (clientSock < 0) 
    {
        std::cerr << "Error: new connection denied." << std::endl;
        return;
    }
    fcntl(clientSock, F_SETFL, O_NONBLOCK);

    struct pollfd pfd;
    pfd.fd = clientSock;
    pfd.events = POLLIN;
    _fds.push_back(pfd);

    // cree et ajoute un  client à la map des utilisateurs, test !!
    Client* newClient = new Client(clientSock, "defaultNickname", "defaultUsername");
    _users[clientSock] = newClient;

    std::cout << "New connection accepted: FD=" << clientSock << std::endl;
}



void Server::handleClient(int fd)
{
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));

    int nbytes = recv(fd, buffer, sizeof(buffer), 0);
    if (nbytes <= 0)
    {
        if (nbytes == 0) 
            std::cout << "USER FD=" << fd << " déconnecté." << std::endl;
        else 
            std::cerr << "ERROR : user could not be reach FD=" << fd << std::endl;
        close(fd); // Ferme le socket client
        closeClient(fd); // Supprime le client de toutes les structures de données
    } 
    else 
        std::cout << "Message received from FD=" << fd << ": " << buffer << std::endl;
        
        // rajout gestion de commandes ???
}



void Server::closeClient(int fd) {
    _fds.erase(std::remove_if(_fds.begin(), _fds.end(), [fd](const struct pollfd& pfd) { return pfd.fd == fd; }), _fds.end());

    // Supprime le client des canaux
    for (auto& channelPair : channels) 
        channelPair.second->removeClient(fd);

    // Supprime le client de la map des utilisateurs et libérer la mémoire
    if (_users.find(fd) != _users.end()) {
        delete _users[fd];
        _users.erase(fd);
    }

    std::cout << "USER FD=" << fd << " correctly removed." << std::endl;
}
