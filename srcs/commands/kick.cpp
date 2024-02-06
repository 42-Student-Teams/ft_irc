/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 14:30:08 by ndiamant          #+#    #+#             */
/*   Updated: 2024/02/06 13:28:23 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"
#include "../../includes/replies.hpp"

/*
      Command: KICK
   Parameters: <channel> <user> [<comment>]

   The KICK command can be  used  to  forcibly  remove  a  user  from  a
   channel.   It  'kicks  them  out'  of the channel (forced PART).



Oikarinen & Reed                                               [Page 25]

RFC 1459              Internet Relay Chat Protocol              May 1993


   Only a channel operator may kick another user out of a  channel.
   Each  server that  receives  a KICK message checks that it is valid
   (ie the sender is actually a  channel  operator)  before  removing
   the  victim  from  the channel.

   Numeric Replies:

           OK ERR_NEEDMOREPARAMS              OK ERR_NOSUCHCHANNEL
           ERR_BADCHANMASK                 OK ERR_CHANOPRIVSNEEDED
           OK ERR_NOTONCHANNEL

   Examples:

KICK &Melbourne Matthew         ; Kick Matthew from &Melbourne

KICK #Finnish John :Speaking English
                                ; Kick John from #Finnish using
                                "Speaking English" as the reason
                                (comment).

:WiZ KICK #Finnish John         ; KICK message from WiZ to remove John
                                from channel #Finnish
*/

void handleKickCommand(const char* message, Users *sender, Server *server)
{
	std::string kickMessage = message;
	std::istringstream iss(kickMessage);
	std::string command, channelName, username, comment;

	if (!(iss >> command >> channelName >> username))
	{
		send(sender->getSocket(), ERR_NEEDMOREPARAMS(sender->getNickname(), command).c_str(),
			ERR_NEEDMOREPARAMS(sender->getNickname(), command).length(), 0);
		std::cout << "Not enough parameters" << std::endl;
		return;
	}
	std::getline(iss >> std::ws, comment);
	if (comment.empty())
		comment = "You have been kicked";
	Channels* channel = server->getChannelByName(channelName);
	if (!channel)
	{
		send(sender->getSocket(), ERR_NOSUCHCHANNEL(sender->getNickname(), channelName).c_str(),
			ERR_NOSUCHCHANNEL(sender->getNickname(), channelName).size(), 0);
		std::cout << "Channel not found" << std::endl;
		return;
	}
	Users* user = channel->getUserByName(username);
	if (user == NULL)
	{
		send(sender->getSocket(), ERR_USERNOTINCHANNEL(sender->getNickname(), username, channelName).c_str(), 
			ERR_USERNOTINCHANNEL(sender->getNickname(), username, channelName).length(), 0);
		std::cout << "User not in channel" << std::endl;
		return;
	}
	if (channel->getOperator(sender) == NULL)
	{
		send(sender->getSocket(), ERR_CHANOPRIVSNEEDED(sender->getNickname(), channelName).c_str(),
			ERR_CHANOPRIVSNEEDED(sender->getNickname(), channelName).length(), 0);
		std::cout << "Not operator" << std::endl;
		return;
	}
	channel->removeUser(user);
	user->setCurrentChannel(nullptr);
	user->removeChannelByName(channelName);
	send(sender->getSocket(), RPL_KICK(user_id(sender->getNickname(), sender->getUsername()), channelName, username, comment).c_str(),
		RPL_KICK(user_id(sender->getNickname(), sender->getUsername()), channelName, username, comment).length(), 0);
}
