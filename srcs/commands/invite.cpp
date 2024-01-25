/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 15:11:21 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/16 12:44:49 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"

/*
	Command: INVITE
	Parameters: <nickname> <channel>

   The INVITE message is used to invite users to a channel.  The
   parameter <nickname> is the nickname of the person to be invited to
   the target channel <channel>.  There is no requirement that the
   channel the target user is being invited to must exist or be a valid
   channel.  To invite a user to a channel which is invite only (MODE
   +i), the client sending the invite must be recognised as being a
   channel operator on the given channel.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_USERONCHANNEL
           ERR_CHANOPRIVSNEEDED
           RPL_INVITING                    RPL_AWAY

   Examples:

   :Angel INVITE Wiz #Dust         ; User Angel inviting WiZ to channel
                                   #Dust

   INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to
                                   #Twilight_zone
*/

void handleInviteCommand(const char* message, Users *sender, Server *server)
{
	(void) server;
	(void) sender;
	(void) message;
	// std::string inviteMessage(message);
	// std::istringstream iss(inviteMessage);
	// std::string command, nickname, channel;
	// iss >> command >> nickname >> channel;

	// // Check if the sender is a member of the channel
	// if (sender->getCurrentChannel()->getUserByName(sender->getNickname()))
	// {
	// 	// Check if the channel is invite-only and the sender is a channel operator
	// 	if (/*!server->isChannelInviteOnly(channel) || */sender->isOperator(sender))
	// 	{
	// 		// Invite the nickname to the channel
	// 		server->inviteUserToChannel(nickname, channel);
	// 	}
	// 	else
	// 	{
	// 		// Sender is not a channel operator, cannot invite other clients
	// 		// Handle error or send error message to the sender
	// 	}
	// }
	// else
	// {
	// 	// Sender is not a member of the channel, cannot invite other clients
	// 	// Handle error or send error message to the sender
	// }
}
