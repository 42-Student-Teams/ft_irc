/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Registration.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 19:49:12 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/22 12:33:37 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Server.hpp"

// void Server::PASS(int fd, std::string cmd)
// {
// 	Client *cli = getClient(fd);
// 	cmd = cmd.substr(4);
// 	size_t pos = cmd.find_first_not_of("\t\v ");
// 	if(pos < cmd.size())
// 	{
// 		cmd = cmd.substr(pos);
// 		if(cmd[0] == ':')
// 			cmd.erase(cmd.begin());
// 	}
// 	if(pos == std::string::npos || cmd.empty()) 
// 		sendMsg(ERR_NOTENOUGHPARAM(std::string("*")), fd);
// 	else if(!cli->getRegistered())
// 	{
// 		std::string pass = cmd;
// 		if(pass == _pass)
// 			cli->setRegistered(true);
// 		else
//             sendMsg(ERR_INCORPASS(std::string("*")), fd);
// 	}
// 	else
//         sendMsg(ERR_ALREADYREGISTERED(getClient(fd)->getNickName()), fd);
// }

// bool Server::checkNickname(std::string& nickname)
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


// bool Server::areDuplicateNames(std::string& nickname)
// {
// 	for (size_t i = 0; i < _clients.size(); i++)
// 	{
// 		if (_clients[i].getNickName() == nickname)
// 			return true;
// 	}
// 	return false;
// }


// void Server::NICK(std::string cmd, int fd)
// {
// 	std::string inuse;
// 	cmd = cmd.substr(4);
// 	size_t pos = cmd.find_first_not_of("\t\v ");
// 	if(pos < cmd.size())
// 	{
// 		cmd = cmd.substr(pos);
// 		if(cmd[0] == ':')
// 			cmd.erase(cmd.begin());
// 	}
// 	Client *cli = getClient(fd);
// 	if(pos == std::string::npos || cmd.empty())
// 		{sendMsg(ERR_NOTENOUGHPARAM(std::string("*")), fd); return;}
// 	if (areDuplicateNames(cmd) && cli->getNickName() != cmd){
// 		inuse = "*";
// 		if(cli->getNickName().empty())
// 			cli->setNickName(inuse);
// 	    sendMsg(ERR_NICKINUSE(std::string(cmd)), fd); 
// 		return;
// 	}
// 	if(!checkNickname(cmd)) {
// 		sendMsg(ERR_ERRONEUSNICK(std::string(cmd)), fd);
// 		return;
// 	}
// 	else
// 	{
// 		if(cli && cli->getRegistered())
// 		{
// 			std::string oldnick = cli->getNickName();
// 			cli->setNickName(cmd);
// 			if(!oldnick.empty() && oldnick != cmd)
// 			{
// 				if(oldnick == "*" && !cli->getUserName().empty())
// 				{
// 					cli->setLogedin(true);
// 					sendMsg(RPL_CONNECTED(cli->getNickName()), fd);
// 					sendMsg(RPL_NICKCHANGE(cli->getNickName(),cmd), fd);
// 				}
// 				else
// 					sendMsg(RPL_NICKCHANGE(oldnick,cmd), fd);
// 				return;
// 			}
			
// 		}
// 		else if (cli && !cli->getRegistered())
// 			sendMsg(ERR_NOTREGISTERED(cmd), fd);
// 	}
// 	if(cli && cli->getRegistered() && !cli->getUserName().empty() && !cli->getNickName().empty() && cli->getNickName() != "*" && !cli->getLogedIn())
// 	{
// 		cli->setLogedin(true);
// 		sendMsg(RPL_CONNECTED(cli->getNickName()), fd);
// 	}
// }

// void	Server::USER(std::string& cmd, int fd)
// {
// 	std::vector<std::string> splited_cmd = parseCmd(cmd);

// 	Client *cli = getClient(fd); 
// 	if((cli && splited_cmd.size() < 5))
// 		{sendMsg(ERR_NOTENOUGHPARAM(cli->getNickName()), fd); return; }
// 	if(!cli  || !cli->getRegistered())
// 		sendMsg(ERR_NOTREGISTERED(std::string("*")), fd);
// 	else if (cli && !cli->getUserName().empty())
// 		{sendMsg(ERR_ALREADYREGISTERED(cli->getNickName()), fd); return;}
// 	else
// 		cli->setUserName(splited_cmd[1]);
// 	if(cli && cli->getRegistered() && !cli->getUserName().empty() && !cli->getNickName().empty() && cli->getNickName() != "*"  && !cli->getLogedIn())
// 	{
// 		cli->setLogedin(true);
// 		sendMsg(RPL_CONNECTED(cli->getNickName()), fd);
// 	}
// }