/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:59:42 by ndiamant          #+#    #+#             */
/*   Updated: 2024/02/08 14:27:54 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"
#include "../../includes/replies.hpp"


/*
      Command: PRIVMSG
   Parameters: <receiver>{,<receiver>} <text to be sent>

   PRIVMSG is used to send private messages between users.  <receiver>
   is the nickname of the receiver of the message.  <receiver> can also
   be a list of names or channels separated with commas.

   The <receiver> parameter may also me a host mask  (#mask)  or  server
   mask  ($mask).   In  both cases the server will only send the PRIVMSG
   to those who have a server or host matching the mask.  The mask  must
   have at  least  1  (one)  "."  in it and no wildcards following the
   last ".".  This requirement exists to prevent people sending messages
   to  "#*"  or "$*",  which  would  broadcast  to  all  users; from
   experience, this is abused more than used responsibly and properly.
   Wildcards are  the  '*' and  '?'   characters.   This  extension  to
   the PRIVMSG command is only available to Operators.

   Numeric Replies:

           ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
           ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
           ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
           ERR_NOSUCHNICK
           RPL_AWAY
*/

void handleMessageCommand(const char* message, Users* sender, Server* server)
{
	std::string privmsg = message;
	std::istringstream iss(privmsg);
	std::string command, receivers, content;
	
	// if (!(iss >> command >> receivers))
	// {
	// 	send(sender->getSocket(), ERR_NORECIPIENT(sender->getNickname()).c_str(),
	// 		ERR_NORECIPIENT(sender->getNickname()).length(), 0);
	// 	return;
	// }

	// std::getline(iss, content);
	// if (content.empty() || content[0] != ':')
	// {
	// 	send(sender->getSocket(), ERR_NOTEXTTOSEND(sender->getNickname()).c_str(),
	// 		ERR_NOTEXTTOSEND(sender->getNickname()).length(), 0);
	// 	return;
	// }
	content.erase(0, 1);

	std::istringstream receiverStream(receivers);
	std::string receiver;
	bool messageSent = false;

				std::cout << "0" << std::endl;
	while (std::getline(receiverStream, receiver, ','))
	{
		std::cout << "00" << std::endl;
		if (receiver.empty()) continue;

		if (receiver[0] == '#' || receiver[0] == '&') // Channel message
		{
			Channels* channel = server->getChannelByName(receiver);
			if (channel)
			{
				std::cout << "1" << std::endl;
				channel->broadcastMessage(content, *sender);
				messageSent = true;
			}
			else
			{
				send(sender->getSocket(), ERR_NOSUCHNICK(sender->getNickname(), receiver).c_str(),
					ERR_NOSUCHNICK(sender->getNickname(), receiver).length(), 0);
			}
		}
		else // User message
		{
			Users* user = server->getUserByNickname(receiver);
			if (user)
			{
				std::string formattedMessage = sender->getNickname() + " messaged you: " + content + "\r\n";
				send(user->getSocket(), formattedMessage.c_str(), formattedMessage.size(), 0);
				messageSent = true;
			}
			else
			{
				send(sender->getSocket(), ERR_NOSUCHNICK(sender->getNickname(), receiver).c_str(),
					ERR_NOSUCHNICK(sender->getNickname(), receiver).length(), 0);
			}
		}
	}

	if (!messageSent)
	{
		send(sender->getSocket(), ERR_NORECIPIENT(sender->getNickname()).c_str(),
			ERR_NORECIPIENT(sender->getNickname()).length(), 0);
	}
}
