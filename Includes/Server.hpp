/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Probook <Probook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:03:48 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/25 16:10:24 by inaranjo         ###   ########.fr       */
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
    
        static void                  handleSignal(int signum);

        Channel*                     createChannel(const std::string& name, const std::string& key, Client* client);

        int                          getFD();
        int                          getPort();
        std::string                  getPass();
        Client                      *getClient(int fd);
        Client                      *getNickClient(std::string nickname);
        Channel                     *getChannel(std::string name);
        std::vector<Channel>&        getChannels();

        void                        setFD(int server_fdsocket);
        void                        setPort(int port);
        void                        setPass(std::string password);
        

        void                        storeClient(Client newClient);
        void                        storeChannel(Channel newChannel);
        void                        storePfds(pollfd newFd);
       
       
        void                        rmClient(int fd);
        void                        rmFds();
        void                        rmPfds(int fd);
        void                        rmClientFromChan(int fd);
        void                        rmChannel(std::string name);
        
       
        void                        sendMsg(std::string msg, int fd);
        void                        sendErrToClient(int code, std::string clientname, int fd, std::string msg);
        void                        sendErrInChannel(int code, std::string clientname, std::string channelname, int fd, std::string msg);
      
    
        void                        run(int port, std::string pass);
        void                        createSocket();
        void                        handleClientConnection();
        void                        handleClientInput(int fd);
        bool                        checkAuth(int fd);
        void                        handleClientDisconnect(int fd);

        

        std::vector<std::string>    parseBuffer(std::string str);
        std::vector<std::string>    parseCmd(std::string &str);
        void                        execCmd(std::string &cmd, int fd);
       
       /*-------------------------COMMANDES SETTINGS-----------------------------------*/
        bool isNicknameInUse(std::string& nickname);
        void handleNickCollision(std::string& nickname);
        

        void                        PASS(int fd, std::string pass);
        void                        NICK(std::string cmd, int fd);
        void                        USER(std::string &username, int fd);
        void                        QUIT(std::string cmd, int fd);

        //---------------------------//JOIN CMD
        void JOIN(std::string cmd, int fd);
        int SplitJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd);
        int SearchForClients(std::string nickname);
        //---------------------------//PART CMD
        void PART(std::string cmd, int fd);
        int SplitCmdPart(std::string cmd, std::vector<std::string> &tmp, std::string &reason, int fd);
        //---------------------------//CKIK CMD
        void KICK(std::string cmd, int fd);
        std::string SplitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd);
        //---------------------------//PRIVMSG CMD
        void PRIVMSG(std::string cmd, int fd);
        void CheckForChannels_Clients(std::vector<std::string> &tmp, int fd);

        //---------------------------//MODE CMD
        void MODE(std::string &cmd, int fd);
        std::string invite_only(Channel *channel, char opera, std::string chain);
        std::string topic_restriction(Channel *channel, char opera, std::string chain);
        std::string password_mode(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string &arguments);
        std::string operator_privilege(std::vector<std::string> splited, Channel *channel, size_t &pos, int fd, char opera, std::string chain, std::string &arguments);
        std::string channel_limit(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string &arguments);
        bool isvalid_limit(std::string &limit);
        std::string mode_toAppend(std::string chain, char opera, char mode);
        std::vector<std::string> splitParams(std::string params);
        void getCmdArgs(std::string cmd, std::string &name, std::string &modeset, std::string &params);
        //---------------------------//TOPIC CMD
        std::string tTopic();
        void Topic(std::string &cmd, int &fd);
        void Invite(std::string &cmd, int &fd);
        std::string gettopic(std::string &input);
        int getpos(std::string &cmd);
};

#endif