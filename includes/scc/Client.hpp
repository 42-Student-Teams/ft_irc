/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 23:17:15 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/12 11:18:16 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/socket.h>
#include <sys/poll.h>

#include <string>
#include <vector>

class Client;

#include "Channel.hpp"
#include "Reply.hpp"

#define BLUE "\033[34m"
#define RESET  "\e[0m"

enum ClientState
{
    HANDSHAKE,// Iinitialisation between server/client status
    LOGIN,// Connection status client
    REGISTERED,// Regirstration status client 
    DISCONNECTED// Disconnection status client
};

class Client 
{
    private:
        int             _fd;
        int             _port;
        std::string     _nickName;
        std::string     _userName;
        std::string     _realName;
        std::string     _hostName;

        ClientState     _state;
        Channel*        _channel;

        Client();
        Client(const Client &src);

    public:
        Client(int fd, int port, const std::string &hostname);
        ~Client();
        
        /*ACCESSORS */
        int             getFd() const;
        int             getPort() const;
        std::string     getNickname() const;
        std::string     getUsername() const;
        std::string     getRealname() const;
        std::string     getHostname() const;
        std::string     getPrefix() const;
        Channel*        getChannel() const;
        std::string     getInfo() const;

        void            setNickname(const std::string &nickname);
        void            setUsername(const std::string &username);
        void            setRealname(const std::string &realname);
        void            setState(ClientState state);
        void            setChannel(Channel *channel);

        /*CHEKC IF CLIENT IS STOCK IN MAP */
        bool            registrationCheck() const;

        /*PARSING MSG/CMD FROM CLIENT */
        void            write(const std::string& message) const;
        void            reply(const std::string& reply);
        void            welcome();

        /*IF REGISTERED : ACCES TO CHAN */
        void            join(Channel *channel);
        void            leave();
};

#endif
