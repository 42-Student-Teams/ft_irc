/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:42:49 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/26 16:21:49 by ndiamant         ###   ########.fr       */
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

           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_NOTONCHANNEL

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
		channelName = msg.substr(spacePos + 1);
	}
	else
	{
		// Handle error: Invalid command format
		return;
	}

	Channels* channel = server->getChannelByName(channelName);
	if (channel)
	{
		channel->removeUser(sender);
		sender->setCurrentChannel(nullptr);
	}
	else
	{
		// Handle error: Channel not found
	}
	std::string reason = "your reason here";
	// send(sender->getFd().fd, RPL_PART(sender->getNickname(), channelName, reason).c_str(),
	// 	RPL_PART(sender->getNickname(), channelName, reason).size() + 1, 0);
	std::string msgs = ":" + sender->getNickname() + "!~" + sender->getUsername() 
		+"@localhost PART " + channelName + " :" + reason + "\r\n";
	send(sender->getFd().fd, msgs.c_str(), msgs.size() + 1, 0);
	channel->broadcastMessage(msgs, *sender);
}
