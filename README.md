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
```

### Logic du Client :
```
Constructeur et destructeur :
Constructeur : Client::Client(int fd, int port, const std::string &hostname)
Destructeur : Client::~Client()
Accesseurs :
Récupération du descripteur de fichier (fd) : Client::getFd()
Récupération du port : Client::getPort()
Récupération du pseudonyme (nickname) : Client::getNickname()
Récupération du nom d'utilisateur (username) : Client::getUsername()
Récupération du nom réel (realname) : Client::getRealname()
Récupération du nom d'hôte (hostname) : Client::getHostname()
Récupération du canal auquel le client est connecté : Client::getChannel()
Récupération du préfixe utilisé dans les messages : Client::getPrefix()
Récupération des informations du client : Client::getInfo()
Mutateurs :
Définition du pseudonyme (nickname) : Client::setNickname(const std::string &nickname)
Définition du nom d'utilisateur (username) : Client::setUsername(const std::string &username)
Définition du nom réel (realname) : Client::setRealname(const std::string &realname)
Définition de l'état du client : Client::setState(ClientState state)
Définition du canal auquel le client est connecté : Client::setChannel(Channel *channel)
Actions réseau du client :
Vérification de l'enregistrement du client : Client::registrationCheck()
Envoi d'un message au client : Client::write(const std::string& message)
Actions du client dans le réseau :
Réponse à un message : Client::reply(const std::string& reply)
Envoi d'un message de bienvenue : Client::welcome()
Rejoindre un canal : Client::join(Channel* channel)
Quitter un canal : Client::leave()
```

### Logic des Commandes :
```
Classe de base abstraite Command :
Cette classe est abstraite, ce qui signifie qu'elle ne peut pas être instanciée directement, mais elle sert de base pour d'autres classes.
Elle contient un pointeur vers l'objet Server associé à la commande et un booléen _auth indiquant si l'authentification est requise pour exécuter cette commande.
La fonction execute() est une méthode virtuelle pure, ce qui signifie qu'elle doit être redéfinie dans les classes dérivées.
Classes dérivées pour les différentes commandes :
Chaque classe dérivée de Command représente une commande spécifique du serveur (par exemple, Quit, User, Nick, etc.).
Chaque classe dérivée redéfinit la fonction execute(), implémentant le comportement spécifique de la commande.
Certaines classes dérivées ont des constructeurs et destructeurs propres pour effectuer des opérations spécifiques à la création et à la destruction de l'objet.
Fonctionnement des commandes :
Lorsqu'une commande est reçue par le serveur, le serveur détermine quelle classe de commande correspond à cette commande en fonction de son nom.
Une instance de la classe correspondante est créée, et la fonction execute() de cette instance est appelée pour effectuer l'action associée à la commande.
La fonction execute() reçoit un pointeur vers l'objet Client qui a émis la commande et un vecteur d'arguments contenant les paramètres de la commande.
```

### Logic des Status client : 
```
Initialisation du client :
Le client est créé avec des informations de base telles que le descripteur de fichier, le port, le nom d'utilisateur, etc.
Son état initial est défini comme CONNECTED.
Changement d'état :
Le serveur peut modifier l'état du client en appelant la méthode setState, en fonction des actions du client ou du serveur. Par exemple, lorsque le client se connecte avec succès, son état passe de CONNECTED à LOGIN.
Les différents états possibles sont définis par l'énumération ClientState.
Vérification de l'état :
La méthode registrationCheck permet de vérifier si le client est enregistré ou non. Cela peut être utilisé par le serveur pour autoriser ou refuser certaines actions en fonction de l'état du client.
Accès restreint aux fonctionnalités :
Certaines fonctionnalités du serveur, telles que rejoindre ou quitter un canal (Channel), sont restreintes en fonction de l'état du client. Par exemple, la méthode join pour rejoindre un canal est accessible uniquement aux clients dans l'état REGISTERED.
Si un client tente d'accéder à une fonctionnalité pour laquelle il n'est pas dans l'état approprié, le serveur peut refuser la demande en fonction de son état actuel.
```
