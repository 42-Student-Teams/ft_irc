/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 23:16:29 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/18 14:40:45 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scc/Client.hpp"

/*--------------------------CONSTRUCT && DESTRUCT----------------------------*/

Client::Client(int fd, int port, const std::string &hostname): _fd(fd), _port(port), _hostName(hostname), _state(CONNECTED), _channel(NULL) {}

Client::~Client() {}

/*--------------------------ACCESSORS----------------------------*/

int             Client::getFd() const { return _fd; }
int             Client::getPort() const { return _port; }
std::string     Client::getNickname() const { return _nickName; }
std::string     Client::getUsername() const { return _userName; }
std::string     Client::getRealname() const { return _realName; }
std::string     Client::getHostname() const { return _hostName; }
Channel*        Client::getChannel() const { return _channel; }

std::string Client::getPrefix() const 
{
    std::string prefix = _nickName;

    if (!_userName.empty())
        prefix += "!" + _userName;
    if (!_hostName.empty())
        prefix += "@" + _hostName;
    return prefix;
}

std::string Client::getInfo() const {
    std::string info = "Nick: " + getNickname() + ", ";
    info += "User: " + getUsername() + ", ";
    info += "Real name: " + getRealname() + ", ";
    info += "Host: " + getHostname();
    return info;
}

void            Client::setNickname(const std::string &nickname) { _nickName = nickname; }
void            Client::setUsername(const std::string &username) { _userName = username; }
void            Client::setRealname(const std::string &realname) { _realName = realname; }
void            Client::setState(ClientState state) { _state = state; }
void            Client::setChannel(Channel *channel) { _channel = channel; }

//new version
void            Client::setIpAdd(std::string addIP){this->_addIP = addIP;}
void            Client::setFD(int fd){this->_fd = fd;}

/*--------------------------CLIENT NETWORK ACTIONS SETUP----------------------------*/

bool            Client::registrationCheck() const { return _state == REGISTERED; }

void            Client::write(const std::string& message) const
{
    std::string buffer = message + "\r\n";
    if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
        throw std::runtime_error("ERROR : msg could not be send!");
}

/*recepetion des messages client*/
void Client::reply(const std::string& reply)
{
    this->write(std::string(BLUE) + ":" + getPrefix() + std::string(RESET) + " " + reply);
}


/*Envoie un message de bienvenue si le client et bien enregistrer*/
void Client::welcome()
{
    if (_state != LOGIN || _userName.empty() || _realName.empty() || _nickName.empty())
		return;
    
    _state = REGISTERED;
    this->reply(RPL_WELCOME(_nickName));

    std::string message = _hostName + ":" + std::to_string(_port) + " nickname setup as: " + _nickName + ".";
    serverON(message);
}

/*client joining channel message, under construction*/
void            Client::join(Channel* channel)
{
    channel->addClient(this);
    _channel = channel;

    // Get users on the channel

    std::string users = "";
    std::vector<std::string> nicknames = channel->getNicknames();
    std::vector<std::string>::iterator it_b = nicknames.begin();
    std::vector<std::string>::iterator it_e = nicknames.end();
    while (it_b != it_e)
    {
        users.append(*it_b + " ");
        it_b++;
    }

    // Send replies
    
    reply(RPL_NAMREPLY(_nickName, channel->getName(), users));
    reply(RPL_ENDOFNAMES(_nickName, channel->getName()));
    channel->broadcast(RPL_JOIN(getPrefix(), channel->getName()));

    // log

    std::string message = _nickName + " has joined to the channel " + channel->getName();
    serverON(message);
}

/*channel leaving message,under construction*/
void            Client::leave()
{
    if (!_channel)
        return;

    const std::string name = _channel->getName();

    _channel->broadcast(RPL_PART(getPrefix(), _channel->getName()));
    _channel->removeClient(this);

    std::string message = _nickName + " has left the channel " + name;
    serverON(message);
}
