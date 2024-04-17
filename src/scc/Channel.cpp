/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Probook <Probook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 23:16:38 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/17 11:49:25 by Probook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scc/Channel.hpp"

/*------------------------------CONSTRUCT && DESTRUCT-------------------------------*/
Channel::Channel(const std::string& name, const std::string& key, Client* admin): _name(name), _admin(admin), _keyAcces(key), _maxUsers(0), _n(false){}

Channel::~Channel() {}

/*------------------------------ACCESSORS-------------------------------*/
std::string                 Channel::getName() const { return _name; }
Client*                     Channel::getAdmin() const { return _admin; }
size_t                      Channel::getSize()const { return _clients.size(); }
size_t                      Channel::getLimit() const { return _maxUsers; }
std::string                 Channel::getKey() const { return _keyAcces; }

std::vector<std::string> Channel::getNicknames()
{
    std::vector<std::string> nicknames;

    std::vector<Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        Client* client = *it;

        std::string nick = (client == _admin ? "@" : "") + client->getNickname();
        nicknames.push_back(nick);
    }

    return nicknames;
}

std::string Channel::getTopic() const {
    return _topic;
}

std::vector<Client*> Channel::getClients() const {
    return _clients;
}

bool                        Channel::extMsg() const { return _n; }

void    Channel::setKey(std::string key) { _keyAcces = key; }
void    Channel::setLimit(size_t limit) { _maxUsers = limit; }
void    Channel::setExtMsg(bool flag) { _n = flag; }

bool Channel::isClientInChannel(Client* client) const {
    for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (*it == client) {
            return true;
        }
    }
    return false;
}

bool Channel::isOperator(Client* client) const {
    return std::find(_operators.begin(), _operators.end(), client) != _operators.end();
}

/*------------------------------CHANNEL SETUP-------------------------------*/
/*------------------------------UNDER CONSTRUCTION-------------------------------*/

void    Channel::addClient(Client* client){ _clients.push_back(client); }

void Channel::addOperator(Client* client) {
    if (std::find(_operators.begin(), _operators.end(), client) == _operators.end()) {
        _operators.push_back(client);
    }
}

void Channel::removeOperator(Client* client) {
    _operators.erase(std::remove(_operators.begin(), _operators.end(), client), _operators.end());
}

void Channel::broadcast(const std::string& message)
{
    std::vector<Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        (*it)->write(message);
    }
}

void Channel::broadcast(const std::string& message, Client* remove)
{
    std::vector<Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == remove)
        {
            continue;
        }

        (*it)->write(message);
    }
}

void Channel::removeClient(Client* client)
{
    std::vector<Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == client)
        {
            _clients.erase(it);
            break;
        }
    }

    client->setChannel(NULL);

    if (client == _admin)
    {
        _admin = *(_clients.begin());

        std::string message = client->getNickname() + " is now the new admin " + _name;
        serverON(message);
    }
}

void    Channel::kick(Client* client, Client* target, const std::string& cause)
{
    this->broadcast(RPL_KICK(client->getPrefix(), _name, target->getNickname(), cause));
    this->removeClient(target);

    std::string message = client->getNickname() + " kicked " + target->getNickname() + " from channel " + _name;
    serverON(message);
}

void Channel::setTopic(const std::string& topic) {
    this->_topic = topic;
}

void Channel::setMode(Client* client, const std::string& modes, const std::string& param, Server* server) {
    if (!isOperator(client)) {
        client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), _name));
        return;
    }

    bool adding = modes[0] == '+'; // + ca veut dire ajoutés - retirés
    // i++ a travers les modes apres + ou -
    for (size_t i = 1; i < modes.size(); ++i) {
        char mode = modes[i];

        if (mode == 'i') { //activer ou desactiver mode invitation 
            _inviteOnly = adding;
        } else if (mode == 't') { //topic modifications
            _topicSettableByOpOnly = adding;
        } else if (mode == 'k') { //mdp du canal
            if (adding) setKey(param);
            else setKey("");
        } else if (mode == 'o') { //add ou retirer operateur 
            Client* target = server->getClient(param);
            if (target) {
                if (adding) addOperator(target);
                else removeOperator(target);
            } else {
                client->reply(ERR_NOSUCHNICK(client->getNickname(), param));
            }
        } else if (mode == 'l') { //limite user 
            if (adding) setLimit(std::stoi(param));
            else setLimit(0);
        } else {
            client->reply(ERR_UNKNOWNMODE(client->getNickname(), std::string(1, mode)));
        }
    }

    // msg de confirmation des changements de mode
    broadcast(":" + client->getPrefix() + " MODE " + _name + " " + modes + " " + param);
}

