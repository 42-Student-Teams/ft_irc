/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 09:58:46 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/22 20:02:05 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Commands.hpp"

//explicit Commands::Commands(Server& server) : _server(server) {}

void Commands::handleCommand(int fd,std::string& command) 
{
    std::vector<std::string> tokens = _server.parseCmd(command);
    if (tokens.empty()) return;
    size_t found = command.find_first_not_of(" \t\v");
	if(found != std::string::npos)
    	command = command.substr(found);

 
   std::string cmdType = tokens[0];
    if (cmdType == "PASS" || cmdType == "pass") {
        handlePASS(fd, command);
    } else if (cmdType == "NICK" || cmdType == "nick") {
        handleNICK(fd, command);
    }
    
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
            _server.sendMsg(RPL_PASSACCEPTED(cli->getNickName()), fd);
        } else {
            _server.sendMsg(ERR_INCORPASS2("*"), fd);
        }
    } else {
        _server.sendMsg(ERR_ALREADYREGISTERED(cli->getNickName()), fd);
    }
}





void Commands::handleNICK(int fd, std::string& command) {
    std::string trimmedCommand = command.substr(4); // Assuming command is prefixed with "NICK "
    size_t pos = trimmedCommand.find_first_not_of("\t\v ");
    if(pos != std::string::npos) {
        trimmedCommand = trimmedCommand.substr(pos);
        if(trimmedCommand[0] == ':') {
            trimmedCommand.erase(0, 1);
        }
    }

    Client* cli = _server.getClient(fd);
    if(trimmedCommand.empty()) {
        _server.sendMsg("Error: No nickname provided.", fd);
        return;
    }

    if(!checkNickname(trimmedCommand)) {
        _server.sendMsg("Error: Invalid nickname.", fd);
        return;
    }

    if(areDuplicateNames(trimmedCommand) && (cli->getNickName() != trimmedCommand)) {
        _server.sendMsg("Error: Nickname already in use.", fd);
        return;
    }

    std::string oldNick = cli->getNickName();
    cli->setNickName(trimmedCommand);
    _server.sendMsg("Nickname changed successfully.", fd);
}

bool Commands::checkNickname(std::string& nickname) {
    if(nickname.empty() || nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':')
        return false;
    for(size_t i = 1; i < nickname.length(); i++) {
        if(!std::isalnum(nickname[i]) && nickname[i] != '_')
            return false;
    }
    return true;
}

bool Commands::areDuplicateNames(std::string& nickname) {
  return _server.isNicknameInUse(nickname);
}











// void Commands::handleNICK(int fd, std::string& cmd) {
//     std::vector<std::string> tokens = _server.parseCmd(cmd);
//     if (tokens.size() < 2) {
//         _server.sendMsg(ERR_NOTENOUGHPARAM2("*"), fd);
//         return;
//     }

//     std::string nickname = tokens[1];
//     Client *cli = _server.getClient(fd);
//     if (nickname.empty() || (areDuplicateNames(nickname) && cli->getNickName() != nickname)) {
//         _server.sendMsg(ERR_NICKINUSE(nickname), fd);
//         return;
//     }

//     if (!checkNickname(nickname)) {
//         _server.sendMsg(ERR_ERRONEUSNICK(nickname), fd);
//         return;
//     }

//     if (cli && cli->getRegistered()) {
//         std::string oldnick = cli->getNickName();
//         cli->setNickName(nickname);
//         _server.sendMsg(RPL_NICKCHANGE(oldnick, nickname), fd);
//     } else {
//         _server.sendMsg(ERR_NOTREGISTERED(nickname), fd);
//     }
// }


// bool Commands::checkNickname(std::string& nickname)
// {
		
// 	if(!nickname.empty() && (nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':'))
// 		return false;
// 	for(size_t i = 1; i < nickname.size(); i++)
// 	{
// 		if(!std::isalnum(nickname[i]) && nickname[i] != '_')
// 			return false;
// 	}
// 	return true;
// }


// bool Commands::areDuplicateNames(std::string& nickname)
// {
// 	for (size_t i = 0; i < _client.getNickName(); i++)
// 	{
// 		if (this->_clients[i].getNickName() == nickname)
// 			return true;
// 	}
// 	return false;
// }