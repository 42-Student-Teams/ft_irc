/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 19:02:18 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/24 14:31:11 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"
#include "../../includes/replies.hpp"

/*
      Command: PING
   Parameters: <server1> [<server2>]

   The PING message is used to test the presence of an active client at
   the other end of the connection.  A PING message is sent at regular
   intervals if no other activity detected coming from a connection.  If
   a connection fails to respond to a PING command within a set amount
   of time, that connection is closed.

   Any client which receives a PING message must respond to <server1>
   (server which sent the PING message out) as quickly as possible with
   an appropriate PONG message to indicate it is still there and alive.
   Servers should not respond to PING commands but rely on PINGs from
   the other end of the connection to indicate the connection is alive.
   If the <server2> parameter is specified, the PING message gets
   forwarded there.

   Numeric Replies:

           ERR_NOORIGIN                    ERR_NOSUCHSERVER

   Examples:

   PING tolsun.oulu.fi             ; server sending a PING message to
                                   another server to indicate it is still
                                   alive.

   PING WiZ                        ; PING message being sent to nick WiZ
*/

void handlePingCommand(const char* message, Users *sender, Server *server)
{
	(void)	server;
	std::string fullMessage(message);

	std::size_t spacePos = fullMessage.find(' ');
	if (spacePos != std::string::npos)
	{
		std::string serverIdentifier = fullMessage.substr(spacePos + 1);

		std::string pongResponse = "PONG " + serverIdentifier + "\r\n";

		send(sender->getSocket(), RPL_PONG(sender->getNickname(), serverIdentifier).c_str(),
			RPL_PONG(sender->getNickname(), serverIdentifier).size(), 0);
	}
}
