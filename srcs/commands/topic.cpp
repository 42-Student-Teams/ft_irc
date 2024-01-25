/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:54:54 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/24 14:40:58 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"

/*
      Command: TOPIC
   Parameters: <channel> [<topic>]

   The TOPIC message is used to change or view the topic of a channel.
   The topic for channel <channel> is returned if there is no <topic>
   given.  If the <topic> parameter is present, the topic for that
   channel will be changed, if the channel modes permit this action.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
           RPL_NOTOPIC                     RPL_TOPIC
           ERR_CHANOPRIVSNEEDED



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
	(void)message;
	(void)sender;
	(void)server;
	std::string channel;
	std::string topic;

	std::istringstream iss(message);
	iss >> channel >> topic;
	if (!channel.empty() && channel[channel.length() - 1] == '\n')
		channel.erase(channel.length() - 1);
	if (!topic.empty() && topic[topic.length() - 1] == '\n')
		topic.erase(topic.length() - 1);
	if (topic.empty())
	{
		std::string toSend = server->getChannelByName(channel)->getTopic() + "\n";
		send(sender->getSocket(), toSend.c_str(), toSend.size() + 1, 0);
		std::cout << server->getChannelByName(channel)->getTopic() << std::endl;
	}
	else if (topic == "-")
	{
		server->getChannelByName(channel)->setTopic("");
	}
	else
	{
		server->getChannelByName(channel)->setTopic(topic);
	}
}
