/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 09:58:46 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/22 20:41:53 by inaranjo         ###   ########.fr       */
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

    std::string& password = tokens[1];  // Mot de passe positionné à l'index 1

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
    std::vector<std::string> tokens = _server.parseCmd(command);

    // check si  NICK a suffisamment de paramètres
    if (tokens.size() < 2) {
        _server.sendMsg(ERR_NONICKNAME2("*"), fd);
        return;
    }

    std::string newNick = tokens[1];
    Client* cli = _server.getClient(fd);

    // Vérifie si le pseudonyme est valide
    if (!checkNickname(newNick)) {
        _server.sendMsg(ERR_ERRONEUSNICK(newNick), fd);
        return;
    }

    // Ignorer le paramètre <hopcount> s'il est fourni

    // Vérifie les collisions de pseudonyme
    if (_server.isNicknameInUse(newNick) && cli->getNickName() != newNick) {
        // En cas de changement de pseudonyme, gére la collision
        if (!cli->getNickName().empty()) {
            _server.handleNickCollision(newNick);
            return;
        }
        // Pour un nouveau pseudonyme
        _server.sendMsg(ERR_NICKNAMEINUSE(newNick), fd);
        return;
    }

    // Définir le nouveau pseudonyme et notifier le changement si nécessaire
    std::string oldNick = cli->getNickName();
    cli->setNickName(newNick);

    // Si c'est un changement de pseudonyme
    if (!oldNick.empty() && oldNick != newNick) {
        _server.sendMsg(RPL_NICKCHANGE(oldNick, newNick), fd);
    } else if (oldNick.empty()) { // Si c'est la première fois que le client est enregistré
        _server.sendMsg(RPL_CONNECTED(newNick), fd); // Bienvenue au nouvel utilisateur
    }
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








