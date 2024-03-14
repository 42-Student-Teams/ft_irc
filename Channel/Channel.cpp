#include "Channel.hpp"

Channel::Channel(const std::string& name) : _ChanName(name) {}

Channel::~Channel(){}

void Channel::addClient(Client* client) 
{
        _users[client->getFd()] = client;
}

void Channel::removeClient(int clientFd)
{
        _users.erase(clientFd);
}

std::string Channel::getName() const { return _ChanName; }