#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include "Client/Client.hpp"

class Channel 
{
    private:
        std::string _ChanName; // Nom du canal
        std::map<int, Client*> _users; // Clients dans ce canal, clé = descripteur de fichier

    public:
        Channel(const std::string& name);
        ~Channel();
        
        void addClient(Client* client);
        void removeClient(int clientFd);
        
        std::string getName() const;


};

#endif