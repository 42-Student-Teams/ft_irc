/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:58:29 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/24 13:36:37 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"

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

	// Check if the user is already registered
	if (sender->isRegistered() == 0)
	{
		// Send error message for already registered users
		std::string errMsg = "ERROR :You are already registered\r\n";
		send(sender->getSocket(), errMsg.c_str(), errMsg.size(), 0);
		return;
	}

	// Verify the password
	if (password != server->getPassword()) {
		// Handle incorrect password case, maybe by closing the connection or sending an error message
		std::string errMsg = "ERROR :Incorrect password\r\n";
		send(sender->getSocket(), errMsg.c_str(), errMsg.size(), 0);
		return;
	}
}
