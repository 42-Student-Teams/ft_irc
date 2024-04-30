/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 09:51:34 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/29 19:07:12 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Client.hpp"

class Server;
class Client;

class Commands
{
    private:
        Server& _server;  // Référence au serveur pour accéder aux clients et autres ressources
        //Client& _client;  // Référence au client pour accéder à ses ressources

        bool checkNickname(std::string& nickname);
        bool areDuplicateNames(std::string& nickname);

    public:
        explicit Commands(Server& server) : _server(server) {}

        void handleCommand( int fd,std::string& command);
        void handlePASS( int fd,std::string& cmd);
        void handleNICK(int fd, std::string& command);
        void handleUSER(int fd, std::string& command);
        void handleJOIN(int fd, std::string& command);
        void handlePART(int fd, std::string& command);
        void handlePRIVMSG(int fd, std::string& command);
        void handleWHO(int fd, std::string& command);
        void handleNOTICE(int fd, std::string& command);
        void handleMODE(int fd, std::string& command);
        void handleTOPIC(int fd, std::string& command);
        void handleQUIT(int fd, std::string& command);
        void handlePING(int fd, std::string& command);
        void handleKICK(int fd, std::string& command);
        // void handleLIST(int fd, std::string& command);

        std::vector<std::string> split(const std::string& s, char delimiter);
};



#endif
