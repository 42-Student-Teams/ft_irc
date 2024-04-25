/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Probook <Probook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 19:49:18 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/25 16:14:51 by Probook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Channel.hpp"

Channel::Channel(Server &server) : _srv(server)
{
    _VIP = 0;
    _topic = 0;
    _key = 0;
    _maxClients = 0;
    _isTopicRestricted = false;
    _name = "";
    _password = "";
    _created_at = "";
    _topicName = "";
    char charaters[5] = {'i', 't', 'k', 'o', 'l'};
    for (int i = 0; i < 5; i++)
        _modes.push_back(std::make_pair(charaters[i], false));
}

Channel::~Channel()
{
}

// Channel::Channel(Channel const &src){
//     *this = src;
// }

Channel &Channel::operator=(Channel const &src)
{
    if (this != &src)
    {
        _VIP = src._VIP;
        _topic = src._topic;
        _key = src._key;
        _maxClients = src._maxClients;
        _isTopicRestricted = src._isTopicRestricted;
        _name = src._name;
        _password = src._password;
        _created_at = src._created_at;
        _topicName = src._topicName;
        _clients = src._clients;
        _admins = src._admins;
        _modes = src._modes;
    }
    return *this;
}

// Setters
void Channel::setInvitation(int _VIP) { this->_VIP = _VIP; }
void Channel::setTopic(int topic) { _topic = topic; }
void Channel::setKey(int key) { _key = key; }
void Channel::setMaxUsers(int limit) { _maxClients = limit; }
void Channel::setTopicName(std::string topic_name) { _topicName = topic_name; }
void Channel::setPass(std::string password) { _password = password; }
void Channel::setName(std::string name) { _name = name; }
void Channel::setTime(std::string time) { _time = time; }

void Channel::setInviteOnly(bool flag) { _inviteOnly = flag; }
void Channel::setTopicControl(bool flag) { _isTopicRestricted = flag; }

// Getters
int Channel::getVIP() { return _VIP; }
int Channel::getTopic() { return _topic; }
int Channel::getKey() { return _key; }
int Channel::getMaxClients() { return _maxClients; }
int Channel::getClientsNumber() { return _clients.size() + _admins.size(); }
bool Channel::getTopicRestric() const { return _isTopicRestricted; }
bool Channel::getModeAtindex(size_t index) { return _modes[index].second; }
std::string Channel::getTopicName() { return _topicName; }
std::string Channel::getPass() { return _password; }
std::string Channel::getName() { return _name; }
std::string Channel::getTime() { return _time; }
std::string Channel::getTimeCreation() { return _created_at; }
std::string Channel::getModes()
{
    std::string mode;
    for (size_t i = 0; i < _modes.size(); i++)
    {
        if (_modes[i].first != 'o' && _modes[i].second)
            mode.push_back(_modes[i].first);
    }
    if (!mode.empty())
        mode.insert(mode.begin(), '+');
    return mode;
}
std::string Channel::getChannelList()
{
    std::string list;
    for (size_t i = 0; i < _admins.size(); i++)
    {
        list += "@" + _admins[i].getNickName();
        if ((i + 1) < _admins.size())
            list += " ";
    }
    if (_clients.size())
        list += " ";
    for (size_t i = 0; i < _clients.size(); i++)
    {
        list += _clients[i].getNickName();
        if ((i + 1) < _clients.size())
            list += " ";
    }
    return list;
}
Client *Channel::getClientFd(int fd)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->getFD() == fd)
            return &(*it);
    }
    return NULL;
}
Client *Channel::getAdminFd(int fd)
{
    for (std::vector<Client>::iterator it = _admins.begin(); it != _admins.end(); ++it)
    {
        if (it->getFD() == fd)
            return &(*it);
    }
    return NULL;
}
Client *Channel::getClientInChannel(std::string name)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->getNickName() == name)
            return &(*it);
    }
    for (std::vector<Client>::iterator it = _admins.begin(); it != _admins.end(); ++it)
    {
        if (it->getNickName() == name)
            return &(*it);
    }
    return NULL;
}

size_t Channel::getSize() const { return _clients.size() + _admins.size(); }

bool Channel::isClientInChannel(std::string &nick)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].getNickName() == nick)
            return true;
    }
    for (size_t i = 0; i < _admins.size(); i++)
    {
        if (_admins[i].getNickName() == nick)
            return true;
    }
    return false;
}

std::vector<Client *> Channel::getClients() const
{
    std::vector<Client *> clientPtrs;
    for (std::vector<Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        clientPtrs.push_back(const_cast<Client *>(&*it));
    }
    return clientPtrs;
}

bool Channel::isOperator(std::string const nickname)
{
    for (std::vector<Client>::iterator it = _admins.begin(); it != _admins.end(); ++it)
    {
        if (it->getNickName() == nickname)
            return true;
    }
    return false;
}

void Channel::storeClient(Client newClient) { _clients.push_back(newClient); }
void Channel::storeAdmin(Client newClient) { _admins.push_back(newClient); }

void Channel::rmClientFd(int fd)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->getFD() == fd)
        {
            _clients.erase(it);
            break;
        }
    }
}
void Channel::rmAdminFd(int fd)
{
    for (std::vector<Client>::iterator it = _admins.begin(); it != _admins.end(); ++it)
    {
        if (it->getFD() == fd)
        {
            _admins.erase(it);
            break;
        }
    }
}

bool Channel::clientTOadmin(std::string &nick)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].getNickName() == nick)
        {
            _admins.push_back(_clients[i]);
            _clients.erase(_clients.begin() + i);
            return true;
        }
    }
    return false;
}
bool Channel::adminTOclient(std::string &nick)
{
    for (size_t i = 0; i < _admins.size(); i++)
    {
        if (_admins[i].getNickName() == nick)
        {
            _clients.push_back(_admins[i]);
            _admins.erase(_admins.begin() + i);
            return true;
        }
    }
    return false;
}

void Channel::sendMsgToAll(std::string rpl1)
{
    for (size_t i = 0; i < _admins.size(); i++)
        if (send(_admins[i].getFD(), rpl1.c_str(), rpl1.size(), 0) == -1)
            std::cerr << "send() faild" << std::endl;
    for (size_t i = 0; i < _clients.size(); i++)
        if (send(_clients[i].getFD(), rpl1.c_str(), rpl1.size(), 0) == -1)
            std::cerr << "send() faild" << std::endl;
}
void Channel::sendMsgToAll(std::string rpl1, int fd)
{
    for (size_t i = 0; i < _admins.size(); i++)
    {
        if (_admins[i].getFD() != fd)
            if (send(_admins[i].getFD(), rpl1.c_str(), rpl1.size(), 0) == -1)
                std::cerr << "send() faild" << std::endl;
    }
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].getFD() != fd)
            if (send(_clients[i].getFD(), rpl1.c_str(), rpl1.size(), 0) == -1)
                std::cerr << "send() faild" << std::endl;
    }
}

void Channel::addOperator(Client *client)
{
    // Check if the client is already an operator
    for (std::vector<Client>::iterator it = _admins.begin(); it != _admins.end(); ++it)
    {
        if (it->getNickName() == client->getNickName())
        {
            return; // Client is already an operator, so do nothing
        }
    }
    _admins.push_back(*client); // Dereference pointer and store the object
}

// void Channel::removeOperator(Client *client)
// {
//     _admins.erase(std::remove(_admins.begin(), _admins.end(), client), _admins.end());
// }

void Channel::removeOperator(Client* client) {
    for (std::vector<Client>::iterator it = _admins.begin(); it != _admins.end(); ++it) {
        if (it->getFD() == client->getFD()) {
            _admins.erase(it);
            break;
        }
    }
}

void Channel::changeOperatorStatus(Client *client, const std::string &targetNick, bool adding)
{
    if (!isOperator(client->getNickName()))
    {
        _srv.sendMsg(ERR_CHANOPRIVSNEEDED(client->getNickName(), targetNick), client->getFD()); // Assurez-vous que getName() est la méthode appropriée pour obtenir le nom du canal.
        return;
    }
    std::string refNick = client->getNickName();

    Client *target = _srv.getClient(client->getFD());
    if (target && isClientInChannel(refNick))
    {
        if (adding)
        {
            addOperator(target);
        }
        else
        {
            removeOperator(target);
        }
    }
    else
    {
        // send_message(ERR_NOSUCHNICK(client->getNickName(), targetNick));
        _srv.sendMsg(ERR_NOSUCHNICK(client->getNickName(), targetNick), client->getFD());
        // client->(ERR_NOSUCHNICK(client->getNickName(), targetNick));
        // Server.-
    }
}

void Channel::broadcastModeChange(const std::string &prefix, const std::string &modes, const std::string &param)
{
    std::string message = ":" + prefix + " MODE " + _name + " " + modes + " " + param;
    sendMsgToAll(message);
}

void Channel::addClient(Client *client)
{
    // Check if the client is already in the channel
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->getNickName() == client->getNickName())
        {
            return; // Client is already in the channel, so do nothing
        }
    }
    _clients.push_back(*client); // Dereference pointer and store the object
}

void Channel::removeClient(Client *client)
{
    // Erase from clients vector
    _clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
    // Optionally, also check and erase from admins if you have different roles
    // _admins.erase(std::remove(_admins.begin(), _admins.end(), client), _admins.end());
}
