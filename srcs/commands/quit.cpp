/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 14:27:08 by ndiamant          #+#    #+#             */
/*   Updated: 2024/02/01 15:12:29 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"

/*
      Command: QUIT
   Parameters: [<Quit message>]

   A client session is ended with a quit message.  The server must close
   the connection to a client which sends a QUIT message. If a "Quit
   Message" is given, this will be sent instead of the default message,
   the nickname.

   When netsplits (disconnecting of two servers) occur, the quit message



Oikarinen & Reed                                               [Page 17]

RFC 1459              Internet Relay Chat Protocol              May 1993


   is composed of the names of two servers involved, separated by a
   space.  The first name is that of the server which is still connected
   and the second name is that of the server that has become
   disconnected.

   If, for some other reason, a client connection is closed without  the
   client  issuing  a  QUIT  command  (e.g.  client  dies and EOF occurs
   on socket), the server is required to fill in the quit  message  with
   some sort  of  message  reflecting the nature of the event which
   caused it to happen.

   Numeric Replies:

           None.

   Examples:

   QUIT :Gone to have lunch        ; Preferred message format.
*/

#include <string>

void handleQuitCommand(const char* message, Users* sender, Server* server)
{
	std::string quitMsg(message ? message : "");
	if (quitMsg.empty())
	{
		quitMsg = "Quit: Client exited";
	}
	else
	{
		if (quitMsg.front() == ':')
				quitMsg.erase(0, 1);
	}

	std::string broadcastMsg = ":" + sender->getNickname() + "!" + sender->getUsername() 
								+ "@localhost QUIT :" + quitMsg;

	std::vector<Channels*> channels = sender->getChannels();
	for (size_t i = 0; i < channels.size(); ++i)
	{
		if (channels[i])
		{
				channels[i]->broadcastMessage(broadcastMsg, *sender);
				channels[i]->removeUser(sender);
		}
	}

	server->closeConnection(sender);
}

