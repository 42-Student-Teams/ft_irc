/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:03:48 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/30 13:40:27 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include "Client.hpp"
#include "Channel.hpp"
#include "Reply.hpp"
#include "Commands.hpp"
#include <string>


#define RED "\e[1;31m"
#define RESET "\e[0;37m"
#define GREEN "\e[1;32m"
#define YELLLOW "\e[1;33m"
#define ORANGE "\e[38;5;214m" 

class Client;
class Channel;

class Server
{
    private:
        static bool                 _signal;
        int                         _port;
        int                         _socketFD;
        std::string                 _pass;
        std::vector<Client>         _clients;
        std::vector<Channel>        _channels;
        std::vector<struct pollfd>  _pfds;
        struct sockaddr_in          _serverAddr;
        struct sockaddr_in          _clientAddr;
        struct pollfd               _newConnection;

    public:
        Server();
        ~Server();
        Server(Server const &src);
        Server &operator=(Server const &src);
    
        /*-------------------------SIGNALS + CHAN-----------------------------------*/
        static void                  handleSignal(int signum);
        Channel*                     createChannel(const std::string& name, const std::string& key, Client* client);

        /*-------------------------GET SETTINGS-----------------------------------*/
        int                          getFD();
        int                          getPort();
        std::string                  getPass();
        Client                      *getClient(int fd);
        Client                      *getNickClient(std::string nickname);
        Channel                     *getChannel(std::string name);
        std::vector<Channel>&        getChannels();

        /*-------------------------SET SETTINGS-----------------------------------*/
        void                        setFD(int server_fdsocket);
        void                        setPort(int port);
        void                        setPass(std::string password);
        
        /*-------------------------STORE SETTINGS-----------------------------------*/
        void                        storeClient(Client newClient);
        void                        storeChannel(Channel newChannel);
        void                        storePfds(pollfd newFd);
       
       /*-------------------------REMOVE SETTINGS-----------------------------------*/
        void                        rmClient(int fd);
        void                        rmFds();
        void                        rmPfds(int fd);
        void                        rmClientFromChan(int fd);
        void                        rmChannel(std::string name);
        
       /*-------------------------MESSAGES SETTINGS-----------------------------------*/
        void                        sendMsg(std::string msg, int fd);
        void                        sendErrToClient(int code, std::string clientname, int fd, std::string msg);
        void                        sendErrInChannel(int code, std::string clientname, std::string channelname, int fd, std::string msg);
        void                        sendWelcome() const;
        static void                 *blinkDots(void* arg);
    
        /*-------------------------SERVER SETTINGS-----------------------------------*/
        void                        run(int port, std::string pass);
        void                        createSocket();
        void                        handleClientConnection();
        void                        handleClientInput(int fd);
        bool                        checkAuth(int fd);
        void                        handleClientDisconnect(int fd);
        
        /*-------------------------PARSER INPUT BUFF-----------------------------------*/
        std::vector<std::string>    parseBuffer(std::string str);
        std::vector<std::string>    parseCmd(std::string &str);
        void                        execCmd(std::string &cmd, int fd);
       
       /*-------------------------COMMANDES SETTINGS-----------------------------------*/
        bool                        isNicknameInUse(std::string& nickname);
        void                        handleNickCollision(std::string& nickname);
        
};

#endif