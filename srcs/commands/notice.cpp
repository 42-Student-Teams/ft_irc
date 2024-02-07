/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 19:02:33 by ndiamant          #+#    #+#             */
/*   Updated: 2024/02/07 10:56:30 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"

/*
      Command: NOTICE
   Parameters: <nickname> <text>

   The NOTICE message is used similarly to PRIVMSG.  The difference
   between NOTICE and PRIVMSG is that automatic replies must never be
   sent in response to a NOTICE message.  This rule applies to servers
   too - they must not send any error reply back to the client on
   receipt of a notice.  The object of this rule is to avoid loops
   between a client automatically sending something in response to
   something it received.  This is typically used by automatons (clients
   with either an AI or other interactive program controlling their
   actions) which are always seen to be replying lest they end up in a
   loop with another automaton.

   See PRIVMSG for more details on replies and examples.
s*/

void handleNoticeCommand(const char* message, Users* sender, Server* server)
{
    std::string noticeMessage = message;
    std::istringstream iss(noticeMessage);
    std::string command, receivers, content;

    if (!(iss >> command >> receivers))
    {
        return;
    }

    std::getline(iss, content);
    if (content.empty() || content[0] != ':')
    {
        return;
    }
    content.erase(0, 1);

    std::istringstream receiverStream(receivers);
    std::string receiver;

    while (std::getline(receiverStream, receiver, ','))
    {
        if (receiver.empty()) continue;

        if (receiver[0] == '#' || receiver[0] == '&') // Channel message
        {
            Channels* channel = server->getChannelByName(receiver);
            if (channel)
            {
                channel->broadcastMessage(content, *sender);
            }
        }
        else // User message
        {
            Users* user = server->getUserByNickname(receiver);
            if (user)
            {
                std::string formattedMessage = sender->getNickname() + " noticed you: " + content + "\r\n";
                send(user->getSocket(), formattedMessage.c_str(), formattedMessage.size(), 0);
            }
        }
    }
}
