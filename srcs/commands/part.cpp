/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:42:49 by ndiamant          #+#    #+#             */
/*   Updated: 2024/02/06 14:07:11 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"
#include "../../includes/replies.hpp"

/*
      Command: PART
   Parameters: <channel>{,<channel>}




Oikarinen & Reed                                               [Page 20]

RFC 1459              Internet Relay Chat Protocol              May 1993


   The PART message causes the client sending the message to be removed
   from the list of active users for all given channels listed in the
   parameter string.

   Numeric Replies:

           OK ERR_NEEDMOREPARAMS              OK ERR_NOSUCHCHANNEL
           OK ERR_NOTONCHANNEL

   Examples:

   PART #twilight_zone             ; leave channel "#twilight_zone"

   PART #oz-ops,&group5            ; leave both channels "&group5" and
                                   "#oz-ops".
*/

void handlePartCommand(const char* message, Users* sender, Server* server)
{
	std::string msg(message);
	std::string channelName;

	size_t spacePos = msg.find(' ');
	if (spacePos != std::string::npos)
	{
		channelName = msg.substr(spacePos + 1, msg.find(' ', spacePos + 1) - spacePos - 1);
	}
	else
	{
		send(sender->getFd().fd, ERR_NEEDMOREPARAMS(user_id(sender->getNickname(), sender->getUsername()), "PART").c_str(),
			ERR_NEEDMOREPARAMS(user_id(sender->getNickname(), sender->getUsername()), "PART").size() + 1, 0);
		std::cout << "Invalid command format" << std::endl;
		return;
	}

	std::string reason;
	size_t colonPos = msg.find(':', spacePos + 1);
	if (colonPos != std::string::npos)
	{
		reason = msg.substr(colonPos + 1);
	}
	else
	{
		reason = "No reason specified";
	}

	Channels* channel = server->getChannelByName(channelName);
	if (channel && channel->getUserByName(sender->getNickname()))
	{
		channel->getUserByName(sender->getNickname())->setCurrentChannel(nullptr);
		sender->removeChannelByName(channelName);
		channel->removeUserByName(sender->getNickname());
		send(sender->getFd().fd, RPL_PART(user_id(sender->getNickname(), sender->getUsername()), channelName, reason).c_str(),
			RPL_PART(user_id(sender->getNickname(), sender->getUsername()), channelName, reason).size() + 1, 0);
	}
	else
	{
		if (!channel)
			send(sender->getFd().fd, ERR_NOSUCHCHANNEL(user_id(sender->getNickname(), sender->getUsername()), channelName).c_str(),
				ERR_NOSUCHCHANNEL(user_id(sender->getNickname(), sender->getUsername()), channelName).size() + 1, 0);
		else
			send(sender->getFd().fd, ERR_NOTONCHANNEL(user_id(sender->getNickname(), sender->getUsername()), channelName).c_str(),
				ERR_NOTONCHANNEL(user_id(sender->getNickname(), sender->getUsername()), channelName).size() + 1, 0);
	}
}
