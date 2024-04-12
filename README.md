# IRC
An IRC Server in C++

### Installation
``` bash
git clone <repository_url>
cd ft_irc
make
```
## Usage example
#### Run this cmd in the terminal
```
./ircserv 4444 hello
```
#### So at this point, the server is running. It just needs to be connected to. You can use any IRC client or simply use nc
```
nc localhost 4444
```
#### and then pass the following commands
```
pass hello
nick lsaba
user lsaba 0 * lsaba
```

### Logic du serveur :
```
Initialisation et fermeture du serveur :
Initialisation du serveur : Server::Server(const std::string &port, const std::string &pass)
Fermeture du serveur : Server::~Server()
Gestion des connexions réseau :
Acceptation des connexions des clients : Server::handleClientConnection()
Gestion des déconnexions des clients : Server::handleClientDisconnect(int fd)
Réception de messages des clients : Server::recvMsgFrom(int fd)
Envoi de messages aux clients : Server::sendMessage(int fd, const std::string& message)
Boucle principale du serveur :
Boucle principale du serveur : Server::run()
Gestion des canaux (channels) :
Création de nouveaux canaux : Server::createChannel(const std::string& name, const std::string& key, Client* client)
Récupération d'un canal spécifique par son nom : Server::getChannel(const std::string& name)
Récupération de tous les canaux existants : Server::getAllChannels()
Gestion des clients :
Récupération du nombre total de clients connectés : Server::getClientCount()
Récupération d'un client spécifique par son pseudo (nickname) : Server::getClient(const std::string& nickname)
Traitement des messages clients :
Traitement des commandes des clients après leur enregistrement : Server::handleClientMessage(int fd)
Fonctionnalités de communication :
Envoi de messages à tous les clients connectés : Server::sendMessage(int fd, const std::string& message)
Autres fonctionnalités :
Conversion de l'ordre des octets réseau en l'ordre des octets de l'hôte : Dans Server::run() avec ntohs(addr.sin_port).
Utilisation de la classe Parser pour analyser les messages des clients : Server::_parser->processMessage(client, message)
Gestion des exceptions et des erreurs pour une expérience de serveur plus robuste : Présent dans toutes les fonctions avec l'utilisation de try, catch, et throw.
Création de sockets et liaison à une adresse IP et un port spécifiés : Server::createSocket()
