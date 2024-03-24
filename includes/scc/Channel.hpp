/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 23:17:20 by inaranjo          #+#    #+#             */
/*   Updated: 2024/03/24 12:03:57 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Channel;

#include "Server.hpp"
#include "Client.hpp"
#include "../Reply.hpp"

class Channel 
{
    private:
        std::string             _name;
        Client*                 _admin;
        std::vector<Client *>   _clients;

        /* Modes => work in progress */
        std::string             _keyAcces; // channel key
        size_t                  _maxUsers; // limit of users
        bool                    _n; // yes/no external messages

        Channel();
        Channel(const Channel& src);
    
    public:
         Channel(const std::string &name, const std::string &key, Client* admin);
        ~Channel();

        /*ACCESSORS*/
        std::string                 getName() const;
        Client*                     getAdmin() const;
        size_t                      getSize() const;
        size_t                      getLimit() const;
        std::string                 getKey() const;
        std::vector<std::string>    getNicknames();
        bool                        extMsg() const;

        void                        setKey(std::string key);
        void                        setLimit(size_t limit);
        void                        setExtMsg(bool flag);

        /*CHANNEL AVALABLE ACTIONS,under construction*/
        void                        addClient(Client* client);
        void                        broadcast(const std::string& message);
        void                        broadcast(const std::string& message, Client* remove);
        void                        removeClient(Client* client);                   
        void                        kick(Client* client, Client* target, const std::string& cause);
};

#endif
