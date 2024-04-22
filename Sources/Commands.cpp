/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 09:58:46 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/22 12:34:47 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Commands.hpp"

void Commands::handleCommand(int fd,std::string& command) 
{
    std::vector<std::string> tokens = _server.parseCmd(command);
    if (tokens.empty()) return;
    size_t found = command.find_first_not_of(" \t\v");
	if(found != std::string::npos)
    	command = command.substr(found);

    std::string cmdType = tokens[0];

    if (cmdType == "PASS" || cmdType == "pass")
        handlePASS(fd,command);
    //suite des cmds...
   
}


void Commands::handlePASS(int fd, std::string& cmd) {
    std::vector<std::string> tokens = _server.parseCmd(cmd);
    Client* cli = _server.getClient(fd);

    // Vérifier si la commande a suffisamment de paramètres et traiter le mot de passe.
    if (tokens.size() < 2) {
        _server.sendMsg(ERR_NOTENOUGHPARAM2("*"), fd);
        return;
    }

    std::string& password = tokens[1];  // Le mot de passe devrait être le second token.

    if (!cli->getRegistered()) {
        if (password == _server.getPass()) {
            cli->setRegistered(true);
        } else {
            _server.sendMsg(ERR_INCORPASS2("*"), fd);
        }
    } else {
        _server.sendMsg(ERR_ALREADYREGISTERED(cli->getNickName()), fd);
    }
}

