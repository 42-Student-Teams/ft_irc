/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:58:29 by ndiamant          #+#    #+#             */
/*   Updated: 2024/02/08 19:51:45 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"
#include "../../includes/replies.hpp"

void handleQuitCommand(const char* message, Users* sender, Server* server);

/*
      Command: PASS
   Parameters: <password>

   The PASS command is used to set a 'connection password'.  The
   password can and must be set before any attempt to register the
   connection is made.  Currently this requires that clients send a PASS
   command before sending the NICK/USER combination and servers *must*
   send a PASS command before any SERVER command.  The password supplied
   must match the one contained in the C/N lines (for servers) or I
   lines (for clients).  It is possible to send multiple PASS commands
   before registering but only the last one sent is used for
   verification and it may not be changed once registered.  Numeric
   Replies:

           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

   Example:

           PASS secretpasswordhere
*/

void handlePassCommand(const char* message, Users *sender, Server *server)
{
	// Extract the password from the message
	std::string password = std::string(message).substr(5); // Assuming "PASS " is 5 characters

	if (password != server->getPassword()) {
		send(sender->getSocket(), ERR_PASSWDMISMATCH(sender->getNickname()).c_str(),
                        ERR_PASSWDMISMATCH(sender->getNickname()).length(), 0);
                if (!sender->isRegistered())
			handleQuitCommand("QUIT", sender, server);
		return;
	}
	sender->setHasEnteredPassword(true);
}
