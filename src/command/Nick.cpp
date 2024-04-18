/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 16:11:13 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/18 17:12:53 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

// syntax: NICK <nickname>

/*
4.1.2 Nick message

      Command: NICK
   Parameters: <nickname> [ <hopcount> ]

   NICK message is used to give user a nickname or change the previous
   one.  The <hopcount> parameter is only used by servers to indicate
   how far away a nick is from its home server.  A local connection has
   a hopcount of 0.  If supplied by a client, it must be ignored.

   If a NICK message arrives at a server which already knows about an
   identical nickname for another client, a nickname collision occurs.
   As a result of a nickname collision, all instances of the nickname
   are removed from the server's database, and a KILL command is issued
   to remove the nickname from all other server's database. If the NICK
   message causing the collision was a nickname change, then the
   original (old) nick must be removed as well.

   If the server recieves an identical NICK from a client which is
   directly connected, it may issue an ERR_NICKCOLLISION to the local
   client, drop the NICK command, and not generate any kills.

   Numeric Replies:

           OK ERR_NONICKNAMEGIVEN             OK ERR_ERRONEUSNICKNAME
           OK ERR_NICKNAMEINUSE               ERR_NICKCOLLISION

   Example:

   NICK Wiz                        ; Introducing new nick "Wiz".

   :WiZ NICK Kilroy                ; WiZ changed his nickname to Kilroy.
*/

Nick::Nick(Server* srv, bool auth) : Command(srv, auth) {}

Nick::~Nick() {}

void    Nick::execute(Client* client, std::vector<std::string> args)
{
    if (args.empty() || args[0].empty()) 
    {
        client->reply(ERR_NONICKNAMEGIVEN(client->getNickname()));
        return;
    }

    std::string nickname = args[0];

    if (_srv->getClientStr(nickname))
    {
        client->reply(ERR_NICKNAMEINUSE(client->getNickname()));
        return;
    }

    client->setNickname(nickname);
    client->reply("NICK registered successfully. Please provide your PASSWORD and USER.");
    client->welcome();
}
