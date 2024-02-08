/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 15:11:21 by ndiamant          #+#    #+#             */
/*   Updated: 2024/02/08 14:06:39 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"
#include "../../includes/replies.hpp"

/*
	Command: INVITE
	Parameters: <nickname> <channel>

   The INVITE message is used to invite users to a channel.  The
   parameter <nickname> is the nickname of the person to be invited to
   the target channel <channel>.  There is no requirement that the
   channel the target user is being invited to must exist or be a valid
   channel.  To invite a user to a channel which is invite only (MODE
   +i), the client sending the invite must be recognised as being a
   channel operator on the given channel.

   Numeric Replies:

           OK ERR_NEEDMOREPARAMS               OK ERR_NOSUCHNICK
           OK ERR_NOTONCHANNEL                OK ERR_USERONCHANNEL
           OK ERR_CHANOPRIVSNEEDED
    	   OK RPL_INVITING                    RPL_AWAY

   Examples:

   :Angel INVITE Wiz #Dust         ; User Angel inviting WiZ to channel
                                   #Dust

   INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to
                                   #Twilight_zone
*/

void handleInviteCommand(const char* message, Users *sender, Server *server)
{
	(void) server;
	(void) sender;
	(void) message;
	std::string inviteMessage(message);
	std::istringstream iss(inviteMessage);
	std::string command, nickname, channel;
	iss >> command >> nickname >> channel;

	if(nickname.empty() || channel.empty())
	{
		send(sender->getSocket(), ERR_NEEDMOREPARAMS(user_id(sender->getNickname(), sender->getUsername()), "INVITE").c_str(), 
			ERR_NEEDMOREPARAMS(user_id(sender->getNickname(), sender->getUsername()), "INVITE").length(), 0);
		return;
	}
	if (server->getChannelByName(channel) == nullptr)
	{
		send(sender->getSocket(), ERR_NOSUCHNICK(user_id(sender->getNickname(), sender->getUsername()), nickname).c_str(), 
			ERR_NOSUCHNICK(user_id(sender->getNickname(), sender->getUsername()), nickname).length(), 0);
		return;
	}
	if (server->getChannelByName(channel)->getUserByName(sender->getNickname()) == nullptr)
	{
		send(sender->getSocket(), ERR_NOTONCHANNEL(user_id(sender->getNickname(), sender->getUsername()), channel).c_str(), 
			ERR_NOTONCHANNEL(user_id(sender->getNickname(), sender->getUsername()), channel).length(), 0);
		return;
	}
	if (server->getUserByNickname(nickname) == nullptr)
	{
		send(sender->getSocket(), ERR_NOSUCHNICK(user_id(sender->getNickname(), sender->getUsername()), nickname).c_str(), 
			ERR_NOSUCHNICK(user_id(sender->getNickname(), sender->getUsername()), nickname).length(), 0);
		return;
	}
	if (server->getChannelByName(channel)->getUserByName(nickname) != nullptr)
	{
		send(sender->getSocket(), ERR_USERONCHANNEL(user_id(sender->getNickname(), sender->getUsername()), nickname, channel).c_str(), 
			ERR_USERONCHANNEL(user_id(sender->getNickname(), sender->getUsername()), nickname, channel).length(), 0);
		return;
	}
	if (sender->getChannelByName(channel) != nullptr)
	{
		if (!sender->getChannelByName(channel)->getInviteOnly() || sender->getChannelByName(channel)->getOperator(sender))
		{
			server->getChannelByName(channel)->addUser(server->getUserByNickname(nickname));
			server->getUserByNickname(nickname)->setCurrentChannel(server->getChannelByName(channel));
			send(server->getUserByNickname(nickname)->getSocket(), RPL_INVITE(user_id(server->getUserByNickname(nickname)->getNickname(), server->getUserByNickname(nickname)->getUsername()), server->getUserByNickname(nickname)->getNickname(), channel).c_str(),
				RPL_INVITE(user_id(server->getUserByNickname(nickname)->getNickname(), server->getUserByNickname(nickname)->getUsername()), server->getUserByNickname(nickname)->getNickname(), channel).length(), 0);
			send(sender->getSocket(), RPL_INVITING(user_id(sender->getNickname(), sender->getUsername()), sender->getUsername(), sender->getNickname(), channel).c_str(),
				RPL_INVITING(user_id(sender->getNickname(), sender->getUsername()), sender->getUsername(), sender->getNickname(), channel).length(), 0);
			send(server->getUserByNickname(nickname)->getSocket(), RPL_JOIN(user_id(server->getUserByNickname(nickname)->getNickname(), server->getUserByNickname(nickname)->getUsername()), channel).c_str(), 
				RPL_JOIN(user_id(server->getUserByNickname(nickname)->getNickname(), server->getUserByNickname(nickname)->getUsername()), channel).length(), 0);
			send(server->getUserByNickname(nickname)->getSocket(), RPL_TOPIC(server->getUserByNickname(nickname)->getNickname(), channel, server->getChannelByName(channel)->getTopic()).c_str(), 
				RPL_TOPIC(server->getUserByNickname(nickname)->getNickname(), channel, server->getChannelByName(channel)->getTopic()).length(), 0);
		}
		else
		{
			send(sender->getSocket(), ERR_CHANOPRIVSNEEDED(user_id(sender->getNickname(), sender->getUsername()), channel).c_str(), 
				ERR_CHANOPRIVSNEEDED(user_id(sender->getNickname(), sender->getUsername()), channel).length(), 0);
			return;
		}
	}
}
