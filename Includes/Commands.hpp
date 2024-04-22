/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 09:51:34 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/22 12:31:46 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Server.hpp"
#include "Client.hpp"

class Server;

class Commands
{
    private:
        Server& _server;  // Référence au serveur pour accéder aux clients et autres ressources

    public:
        explicit Commands(Server& server) : _server(server) {}

        void handleCommand( int fd,std::string& command);
        void handlePASS( int fd,std::string& cmd);

};



#endif
