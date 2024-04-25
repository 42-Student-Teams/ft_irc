/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Probook <Probook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:03:35 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/25 17:36:38 by Probook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Server.hpp"

bool Server::_signal = false;

Server::Server() : _socketFD(-1) {}

Server::~Server() {}

Server::Server(Server const &src) {*this = src;}

Server &Server::operator=(Server const &src) {
    if (this != &src) {
        this->_port = src._port;
        this->_socketFD = src._socketFD;
        this->_pass = src._pass;
        this->_clients = src._clients;
        this->_channels = src._channels;
        this->_pfds = src._pfds;
        this->_serverAddr = src._serverAddr;
        this->_clientAddr = src._clientAddr;
        this->_newConnection = src._newConnection;
    }
    return *this;
}

void Server::handleSignal(int signum) {
    (void)signum;
    std::cout << std::endl << "Signal Received!" << std::endl;
    Server::_signal = true;
}

Channel* Server::createChannel(const std::string& name, const std::string& key, Client* client) {
    Channel newChannel;
    newChannel.setName(name);
    newChannel.setPass(key);

    newChannel.addClient(client);
    
    _channels.push_back(newChannel);

    return &_channels.back();
}

int Server::getFD() { return this->_socketFD; }
int Server::getPort() { return this->_port; }
std::string Server::getPass() { return this->_pass; }
Client *Server::getClient(int fd) {
    for (size_t i = 0; i < this->_clients.size(); i++) {
        if (this->_clients[i].getFD() == fd)
            return &this->_clients[i];
    }
    return nullptr;
}
Client *Server::getNickClient(std::string nickname) {
    for (size_t i = 0; i < this->_clients.size(); i++) {
        if (this->_clients[i].getNickName() == nickname)
            return &this->_clients[i];
    }
    return nullptr;
}
Channel *Server::getChannel(std::string name) {
    for (size_t i = 0; i < this->_channels.size(); i++) {
        if (this->_channels[i].getName() == name)
            return &this->_channels[i];
    }
    return nullptr;
}

// std::vector<Channel*> Server::getAllChannels() {
//     std::vector<Channel*> allChannels;
//     for (Channel& channel : _channels) {
//         allChannels.push_back(&channel);
//     }
//     return allChannels;
// }

void Server::setFD(int fd) { this->_socketFD = fd; }
void Server::setPort(int port) { this->_port = port; }
void Server::setPass(std::string password) { this->_pass = password; }


void Server::storeClient(Client newClient) { this->_clients.push_back(newClient); }
void Server::storePfds(pollfd newFd) { this->_pfds.push_back(newFd); }
void Server::storeChannel(Channel newChannel) { this->_channels.push_back(newChannel); }


void Server::rmClient(int fd) {
    for (size_t i = 0; i < this->_clients.size(); i++) {
        if (this->_clients[i].getFD() == fd) {
            this->_clients.erase(this->_clients.begin() + i);
            return;
        }
    }
}
void Server::rmFds() {
    for (size_t i = 0; i < _clients.size(); i++) {
        std::cout << RED << "Client <" << _clients[i].getFD() << "> Disconnected" << RESET << std::endl;
        close(_clients[i].getFD());
    }
    if (_socketFD != -1) {
        std::cout << RED << "Server <" << _socketFD << "> Disconnected" << RESET << std::endl;
        close(_socketFD);
    }
}
void Server::rmPfds(int fd) {
    for (size_t i = 0; i < this->_pfds.size(); i++) {
        if (this->_pfds[i].fd == fd) {
            this->_pfds.erase(this->_pfds.begin() + i);
            return;
        }
    }
}

void	Server::RmChannels(int fd){
	for (size_t i = 0; i < this->channels.size(); i++){
		int flag = 0;
		if (channels[i].get_client(fd))
			{channels[i].remove_client(fd); flag = 1;}
		else if (channels[i].get_admin(fd))
			{channels[i].remove_admin(fd); flag = 1;}
		if (channels[i].GetClientsNumber() == 0)
			{channels.erase(channels.begin() + i); i--; continue;}
		if (flag){
			// std::string rpl = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost QUIT Quit\r\n";
			// channels[i].sendTo_all(rpl);
		}
	}
}


void Server::rmClientFromChan(int fd) {
    for (size_t i = 0; i < this->_channels.size(); i++) {
        int flag = 0;
        if (this->_channels[i].getClientFd(fd)) {
            this->_channels[i].rmClientFd(fd);
            flag = 1;
        } else if (this->_channels[i].getAdminFd(fd)) {
            this->_channels[i].rmAdminFd(fd);
            flag = 1;
        }
        if (this->_channels[i].getClientsNumber() == 0) {
            this->_channels.erase(this->_channels.begin() + i);
            i--;
            continue;
        }
        if (flag) {
            std::string rpl = ":" + getClient(fd)->getNickName() + "!~" + getClient(fd)->getUserName() + "@localhost QUIT Quit\r\n";
            this->_channels[i].sendMsgToAll(rpl);
        }
    }
}

// void Server::rmChannel(std::string name) {
//     for (size_t i = 0; i < this->_channels.size(); i++) {
//         if (this->_channels[i].getName() == name) {
//             this->_channels.erase(this->_channels.begin() + i);
//             return;
//         }
//     }
// }


void Server::sendMsg(std::string msg, int fd) {
    std::cout << "msg:\n" << msg;
    if (send(fd, msg.c_str(), msg.size(), 0) == -1)
        std::cerr << "msg send() failed" << std::endl;
}
void Server::sendErrToClient(int code, std::string clientname, int fd, std::string msg) {
    std::stringstream ss;
    ss << ":localhost " << code << " " << clientname << msg;
    std::string resp = ss.str();
    if (send(fd, resp.c_str(), resp.size(), 0) == -1)
        std::cerr << "send() failed" << std::endl;
}
void Server::sendErrInChannel(int code, std::string clientname, std::string channelname, int fd, std::string msg) {
    std::stringstream ss;
    ss << ":localhost " << code << " " << clientname << " " << channelname << msg;
    std::string resp = ss.str();
    if (send(fd, resp.c_str(), resp.size(), 0) == -1)
        std::cerr << "send() failed" << std::endl;
}


void Server::run(int port, std::string pass) {
    _pass = pass;
    _port = port;
    createSocket();

    std::cout << "Waiting to accept a connection...\n";
    while (_signal == false) {
        if ((poll(&_pfds[0], _pfds.size(), -1) == -1) && _signal == false)
            throw(std::runtime_error("poll() failed"));
        for (size_t i = 0; i < _pfds.size(); i++) {
            if (_pfds[i].revents & POLLIN) {
                if (_pfds[i].fd == _socketFD)
                    handleClientConnection();
                else
                    handleClientInput(_pfds[i].fd);
            }
        }
    }
    rmFds();
}

void Server::handleClientDisconnect(int fd) {
    std::cout << RED << "Client <" << fd << "> Disconnected" << RESET << std::endl;
    rmClientFromChan(fd);  
    rmClient(fd);
    rmPfds(fd);
    close(fd);
}

void Server::createSocket() {
    int en = 1;
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_addr.s_addr = INADDR_ANY;
    _serverAddr.sin_port = htons(_port);
    _socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFD == -1)
        throw(std::runtime_error("failed to create socket"));
    if (setsockopt(_socketFD, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
        throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
    if (fcntl(_socketFD, F_SETFL, O_NONBLOCK) == -1)
        throw(std::runtime_error("failed to set option (O_NONBLOCK) on socket"));
    if (bind(_socketFD, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) == -1)
        throw(std::runtime_error("failed to bind socket"));
    if (listen(_socketFD, SOMAXCONN) == -1)
        throw(std::runtime_error("listen() failed"));
    _newConnection.fd = _socketFD;
    _newConnection.events = POLLIN;
    _newConnection.revents = 0;
    _pfds.push_back(_newConnection);
}
void Server::handleClientConnection() {
    Client cli;
    memset(&_clientAddr, 0, sizeof(_clientAddr));
    socklen_t len = sizeof(_clientAddr);
    int incofd = accept(_socketFD, (sockaddr *)&_clientAddr, &len);
    if (incofd == -1) {
        std::cout << "accept() failed" << std::endl;
        return;
    }
    if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1) {
        std::cout << "fcntl() failed" << std::endl;
        return;
    }
    _newConnection.fd = incofd;
    _newConnection.events = POLLIN;
    _newConnection.revents = 0;
    cli.setFD(incofd);
    cli.setClientIP(inet_ntoa((_clientAddr.sin_addr)));
    _clients.push_back(cli);
    _pfds.push_back(_newConnection);
    std::cout << GREEN << "Client <" << incofd << "> Connected" << RESET << std::endl;
}
void Server::handleClientInput(int fd) {
    std::vector<std::string> cmd;
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    Client *cli = getClient(fd);
    ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0);
    if (bytes <= 0) {
        std::cout << RED << "Client <" << fd << "> Disconnected" << RESET << std::endl;
        rmClientFromChan(fd);
        rmClient(fd);
        rmPfds(fd);
        close(fd);
    } else { 
        cli->setBuffer(buff);
        if (cli->getBuffer().find_first_of("\r\n") == std::string::npos)
            return;
        cmd = parseBuffer(cli->getBuffer());
        std::cout << "Debug: " << cli->getBuffer();
        for (size_t i = 0; i < cmd.size(); i++)
            execCmd(cmd[i], fd);
        if (getClient(fd))
            getClient(fd)->clearBuffer();
    }
}
bool Server::notRegistered(int fd)
{
	if (!getClient(fd) || getClient(fd)->getNickName().empty() || getClient(fd)->getUserName().empty() || getClient(fd)->getNickName() == "*"  || !getClient(fd)->getLogedIn())
		return false;
	return true;
}


std::vector<std::string> Server::parseBuffer(std::string str)
{
	std::vector<std::string> vec;
	std::istringstream stm(str);
	std::string line;
	while(std::getline(stm, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		vec.push_back(line);
	}
	return vec;
}
std::vector<std::string> Server::parseCmd(std::string& cmd)
{
	std::vector<std::string> vec;
	std::istringstream stm(cmd);
	std::string token;
	while(stm >> token)
	{
		vec.push_back(token);
		token.clear();
	}
	return vec;
}

bool Server::isNicknameInUse( std::string& nickname){
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].getNickName() == nickname)
            return true;
    }
    return false;
}

void Server::handleNickCollision(std::string& nickname) {
    // Parcourir la liste des clients et trouver tous ceux avec le pseudonyme en collision
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ) {
        if (it->getNickName() == nickname) {
            sendMsg(ERR_NICKCOLLISION(nickname), it->getFD());
            it = _clients.erase(it);  // Supprimer le client du vecteur et avancer l'itérateur
        } else {
            ++it;
        }
    }
}


void Server::execCmd( std::string& cmd, int fd)
{
    Commands cmdHandler(*this);
    cmdHandler.handleCommand(fd,cmd);
}


// void Server::execCmd(std::string &cmd, int fd)
// {
// 	if(cmd.empty())
// 		return ;
// 	std::vector<std::string> splited_cmd = parseCmd(cmd);
// 	size_t found = cmd.find_first_not_of(" \t\v");
// 	if(found != std::string::npos)
// 		cmd = cmd.substr(found);
//     if(splited_cmd.size() && (splited_cmd[0] == "PASS" || splited_cmd[0] == "pass"))
//         PASS(fd, cmd);
// 	else if (splited_cmd.size() && (splited_cmd[0] == "NICK" || splited_cmd[0] == "nick"))
// 		NICK(cmd,fd);
// 	else if(splited_cmd.size() && (splited_cmd[0] == "USER" || splited_cmd[0] == "user"))
// 		USER(cmd, fd);
// 	// else if (splited_cmd.size() && (splited_cmd[0] == "QUIT" || splited_cmd[0] == "quit"))
// 	// 	QUIT(cmd,fd);
// 	else if(notRegistered(fd))
// 	{
// 		// if (splited_cmd.size() && (splited_cmd[0] == "KICK" || splited_cmd[0] == "kick"))
// 		// 	KICK(cmd, fd);
// 		// else if (splited_cmd.size() && (splited_cmd[0] == "JOIN" || splited_cmd[0] == "join"))
// 		// 	JOIN(cmd, fd);
// 		// else if (splited_cmd.size() && (splited_cmd[0] == "TOPIC" || splited_cmd[0] == "topic"))
// 		// 	Topic(cmd, fd);
// 		// else if (splited_cmd.size() && (splited_cmd[0] == "MODE" || splited_cmd[0] == "mode"))
// 		// 	MODE(cmd, fd);
// 		// else if (splited_cmd.size() && (splited_cmd[0] == "PART" || splited_cmd[0] == "part"))
// 		// 	PART(cmd, fd);
// 		// else if (splited_cmd.size() && (splited_cmd[0] == "PRIVMSG" || splited_cmd[0] == "privmsg"))
// 		// 	PRIVMSG(cmd, fd);
// 		// else if (splited_cmd.size() && (splited_cmd[0] == "INVITE" || splited_cmd[0] == "invite"))
// 		// 	Invite(cmd,fd);
// 		// else if (splited_cmd.size())
// 		// 	sendMsg(ERR_CMDNOTFOUND(getClient(fd)->getNickName(),splited_cmd[0]),fd);
// 	}
// 	else if (!notRegistered(fd))
// 		sendMsg(ERR_NOTREGISTERED(std::string("*")),fd);
// }
// //---------------//Parsing Methods
