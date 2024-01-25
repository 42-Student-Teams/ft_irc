/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 12:13:07 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/24 18:54:01 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"
#include "../../includes/replies.hpp"

/*
      Command: JOIN
   Parameters: <channel>{,<channel>} [<key>{,<key>}]

   The JOIN command is used by client to start listening a specific
   channel. Whether or not a client is allowed to join a channel is
   checked only by the server the client is connected to; all other
   servers automatically add the user to the channel when it is received
   from other servers.  The conditions which affect this are as follows:

           1.  the user must be invited if the channel is invite-only;




Oikarinen & Reed                                               [Page 19]

RFC 1459              Internet Relay Chat Protocol              May 1993


           2.  the user's nick/username/hostname must not match any
               active bans;

           3.  the correct key (password) must be given if it is set.

   These are discussed in more detail under the MODE command (see
   section 4.2.3 for more details).

   Once a user has joined a channel, they receive notice about all
   commands their server receives which affect the channel.  This
   includes MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.  The
   JOIN command needs to be broadcast to all servers so that each server
   knows where to find the users who are on the channel.  This allows
   optimal delivery of PRIVMSG/NOTICE messages to the channel.

   If a JOIN is successful, the user is then sent the channel's topic
   (using RPL_TOPIC) and the list of users who are on the channel (using
   RPL_NAMREPLY), which must include the user joining.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
           ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
           ERR_CHANNELISFULL               ERR_BADCHANMASK
           ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
           RPL_TOPIC
*/

void handleJoinCommand(const char* message, Users *sender, Server *server)
{
	std::string joinMessage(message);
	if (joinMessage.length() <= 6)
	{
		send(sender->getSocket(), ERR_NEEDMOREPARAMS(sender->getNickname(), "JOIN").c_str(), 
			ERR_NEEDMOREPARAMS(sender->getNickname(), "JOIN").length(), 0);
		return;
	}
	std::string channelName = message + 5;
	if (channelName[0] != '#')
	{
		send(sender->getSocket(), ERR_NOSUCHCHANNEL(sender->getNickname(), channelName).c_str(), 
			ERR_NOSUCHCHANNEL(sender->getNickname(), channelName).length(), 0);
		return;
	}
	if (!channelName.empty() && channelName[channelName.length() - 1] == '\n')
		channelName.erase(channelName.length() - 1);
	Channels* channel = server->getChannelByName(channelName);
	if (!channel)
	{
		server->ensureChannelExists(channelName, sender);
		channel = server->getChannelByName(channelName);
	}
	if (sender->getCurrentChannel())
		sender->getCurrentChannel()->removeUser(sender);
	sender->setCurrentChannel(channel);
	sender->getCurrentChannel()->addUser(sender);
	send(sender->getSocket(), RPL_JOIN(sender->getNickname(), channelName).c_str(), 
		RPL_JOIN(sender->getNickname(), channelName).length(), 0);
	send(sender->getSocket(), RPL_TOPIC(sender->getNickname(), channelName, channel->getTopic()).c_str(), 
		RPL_TOPIC(sender->getNickname(), channelName, channel->getTopic()).length(), 0);
}
