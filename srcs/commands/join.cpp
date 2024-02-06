/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 12:13:07 by ndiamant          #+#    #+#             */
/*   Updated: 2024/02/06 16:06:54 by ndiamant         ###   ########.fr       */
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

           OK ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
           OK ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
           ERR_CHANNELISFULL                  ERR_BADCHANMASK
           OK ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
           OK RPL_TOPIC
*/

void handleJoinCommand(const char* message, Users* sender, Server* server)
{
	std::string joinMessage(message);
	if (joinMessage.length() <= 6)
	{
		send(sender->getSocket(), ERR_NEEDMOREPARAMS(sender->getNickname(), "JOIN").c_str(),
			ERR_NEEDMOREPARAMS(sender->getNickname(), "JOIN").length(), 0);
		return;
	}

	std::istringstream iss(joinMessage.substr(5));
	std::string channelsAndKeys;
	getline(iss, channelsAndKeys);
	size_t keyStart = channelsAndKeys.find(' ');
	std::string channelPart = channelsAndKeys.substr(0, keyStart);
	std::string keyPart = keyStart != std::string::npos ? channelsAndKeys.substr(keyStart + 1) : "";

	std::vector<std::string> channelNames;
	std::vector<std::string> keys;
	std::istringstream channelStream(channelPart);
	std::istringstream keyStream(keyPart);
	std::string channelName;
	std::string key;

	while (getline(channelStream, channelName, ','))
	{
		if (!channelName.empty()) {
			channelNames.push_back(channelName);
		}
	}

	if (!keyPart.empty()) {
		while (getline(keyStream, key, ','))
		{
			keys.push_back(key);
		}
	}

	bool joinedAtLeastOneChannel = false;
	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		channelName = channelNames[i];
		key = i < keys.size() ? keys[i] : "";

		if (channelName.empty() || channelName[0] != '#')
		{
			send(sender->getSocket(), ERR_NOSUCHCHANNEL(sender->getNickname(), channelName).c_str(),
				ERR_NOSUCHCHANNEL(sender->getNickname(), channelName).length(), 0);
			continue;
		}
		Channels* channel = server->getChannelByName(channelName);
		if (!channel)
		{
			server->ensureChannelExists(channelName, sender);
			channel = server->getChannelByName(channelName);
		}
		if (channel->getInviteOnly())
		{
			send(sender->getSocket(), ERR_INVITEONLYCHAN(sender->getNickname(), channelName).c_str(),
				ERR_INVITEONLYCHAN(sender->getNickname(), channelName).length(), 0);
			continue;
		}
		if (channel->getChannelKey() != "" && channel->getChannelKey() != key)
		{
			send(sender->getSocket(), ERR_BADCHANNELKEY(sender->getNickname(), channelName).c_str(),
				ERR_BADCHANNELKEY(sender->getNickname(), channelName).length(), 0);
			continue;
		}
		channel->addUser(sender);
		joinedAtLeastOneChannel = true;
		send(sender->getSocket(), RPL_JOIN(user_id(sender->getNickname(), sender->getUsername()), channelName).c_str(),
			RPL_JOIN(user_id(sender->getNickname(), sender->getUsername()), channelName).length(), 0);
		send(sender->getSocket(), RPL_TOPIC(sender->getNickname(), channelName, channel->getTopic()).c_str(),
			RPL_TOPIC(sender->getNickname(), channelName, channel->getTopic()).length(), 0);
	}
}
