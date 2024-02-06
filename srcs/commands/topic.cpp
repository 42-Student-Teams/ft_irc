/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:54:54 by ndiamant          #+#    #+#             */
/*   Updated: 2024/02/06 14:31:03 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"
#include "../../includes/replies.hpp"

/*
      Command: TOPIC
   Parameters: <channel> [<topic>]

   The TOPIC message is used to change or view the topic of a channel.
   The topic for channel <channel> is returned if there is no <topic>
   given.  If the <topic> parameter is present, the topic for that
   channel will be changed, if the channel modes permit this action.

   Numeric Replies:

           OK ERR_NEEDMOREPARAMS              OK ERR_NOTONCHANNEL
           OK RPL_NOTOPIC                     OK RPL_TOPIC
           OK ERR_CHANOPRIVSNEEDED



Oikarinen & Reed                                               [Page 23]

RFC 1459              Internet Relay Chat Protocol              May 1993


   Examples:

   :Wiz TOPIC #test :New topic     ;User Wiz setting the topic.

   TOPIC #test :another topic      ;set the topic on #test to "another
                                   topic".

   TOPIC #test                     ; check the topic for #test.
*/

void handleTopicCommand(const char* message, Users *sender, Server *server)
{
	std::string msg(message);
	std::istringstream iss(msg);
	std::string command, channelName, topic;
	
	iss >> command >> channelName;
	std::getline(iss, topic);
	if (!topic.empty() && topic[0] == ' ')
	{
		topic.erase(0, 1);
	}

	if (channelName.empty())
	{
		send(sender->getSocket(), ERR_NEEDMOREPARAMS(sender->getNickname(), "TOPIC").c_str(),
			ERR_NEEDMOREPARAMS(sender->getNickname(), "TOPIC").length(), 0);
		return;
	}

	Channels* channel = server->getChannelByName(channelName);
	if (!channel)
	{
		send(sender->getSocket(), ERR_NOSUCHCHANNEL(sender->getNickname(), channelName).c_str(),
			ERR_NOSUCHCHANNEL(sender->getNickname(), channelName).length(), 0);
		return;
	}

	if (!channel->getUserByName(sender->getNickname()))
	{
		send(sender->getSocket(), ERR_NOTONCHANNEL(sender->getNickname(), channelName).c_str(),
			ERR_NOTONCHANNEL(sender->getNickname(), channelName).length(), 0);
		return;
	}

	if (topic.empty())
	{
		const std::string& currentTopic = channel->getTopic();
		if (currentTopic.empty())
		{
			send(sender->getSocket(), RPL_NOTOPIC(sender->getNickname(), channelName).c_str(),
				RPL_NOTOPIC(sender->getNickname(), channelName).length(), 0);
		}
		else
		{
			send(sender->getSocket(), RPL_TOPIC(sender->getNickname(), channelName, currentTopic).c_str(),
				RPL_TOPIC(sender->getNickname(), channelName, currentTopic).length(), 0);
		}
	}
	else
	{
		if (!channel->getOperator(sender) && channel->getTopicRestrictions())
		{
			send(sender->getSocket(), ERR_CHANOPRIVSNEEDED(sender->getNickname(), channelName).c_str(),
				ERR_CHANOPRIVSNEEDED(sender->getNickname(), channelName).length(), 0);
			return;
		}
		channel->setTopic(topic);
		send(sender->getSocket(), RPL_TOPIC(sender->getNickname(), channelName, topic).c_str(),
			RPL_TOPIC(sender->getNickname(), channelName, topic).length(), 0);
	}
}
