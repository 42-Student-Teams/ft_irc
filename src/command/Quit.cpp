/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 16:11:41 by inaranjo          #+#    #+#             */
/*   Updated: 2024/03/24 11:51:56 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

// syntax: QUIT [<message>]

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

Quit::Quit(Server* srv, bool auth) : Command(srv, auth) {}

Quit::~Quit() {}

void    Quit::execute(Client* client, std::vector<std::string> args)
{
    std::string reason = args.empty() ? "Leaving..." : args.at(0);

    if (reason.at(0) == ':')
        reason = reason.substr(1);

    client->write(RPL_QUIT(client->getPrefix(), reason));
	_srv->handleClientDisconnect(client->getFd());
}
