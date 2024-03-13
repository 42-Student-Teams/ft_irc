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
            std::cerr << "Erreur poll." << std::endl;
            break;
        }

        if (_fds[0].revents & POLLIN) 
            acceptNewConnection();

        // Gérer les entrées/sorties des clients ici
    }
}

void Server::acceptNewConnection() 
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSock = accept(_socketFD, (struct sockaddr *)&clientAddr, &clientAddrLen);

    if (clientSock < 0) {
        std::cerr << "Erreur lors de l'acceptation d'une nouvelle connexion." << std::endl;
        return;
    }

    // Configurer le socket client en mode non bloquant
    fcntl(clientSock, F_SETFL, O_NONBLOCK);

    struct pollfd pfd;
    pfd.fd = clientSock;
    pfd.events = POLLIN;
    _fds.push_back(pfd);

    std::cout << "Nouvelle connexion acceptée: FD=" << clientSock << std::endl;

    // À ce stade, vous ajouteriez le nouveau client à votre structure de données client
}