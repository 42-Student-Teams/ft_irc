/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:56:13 by ndiamant          #+#    #+#             */
/*   Updated: 2024/02/06 13:42:03 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"
#include "../../includes/replies.hpp"

/*
4.2.3 Mode message

	Command: MODE

The MODE command is a dual-purpose command in IRC.  It allows both
usernames and channels to have their mode changed.  The rationale for
this choice is that one day nicknames will be obsolete and the
equivalent property will be the channel.

When parsing MODE messages, it is recommended that the entire message
be parsed first and then the changes which resulted then passed on.

4.2.3.1 Channel modes

Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
			[<ban mask>]

The MODE command is provided so that channel operators may change the
characteristics of `their' channel.  It is also required that servers
be able to change channel modes so that channel operators may be
created.

The various modes available for channels are as follows:

		o - give/take channel operator privileges;
		p - private channel flag;
		s - secret channel flag;
		i - invite-only channel flag;
		t - topic settable by channel operator only flag;
		n - no messages to channel from clients on the outside;
		m - moderated channel;
		l - set the user limit to channel;



Oikarinen & Reed                                               [Page 21]

RFC 1459              Internet Relay Chat Protocol              May 1993


		b - set a ban mask to keep users out;
		v - give/take the ability to speak on a moderated channel;
		k - set a channel key (password).

When using the 'o' and 'b' options, a restriction on a total of three
per mode command has been imposed.  That is, any combination of 'o'
and

4.2.3.2 User modes

Parameters: <nickname> {[+|-]|i|w|s|o}

The user MODEs are typically changes which affect either how the
client is seen by others or what 'extra' messages the client is sent.
A user MODE command may only be accepted if both the sender of the
message and the nickname given as a parameter are both the same.

The available modes are as follows:

		i - marks a users as invisible;
		s - marks a user for receipt of server notices;
		w - user receives wallops;
		o - operator flag.

Additional modes may be available later on.

If a user attempts to make themselves an operator using the "+o"
flag, the attempt should be ignored.  There is no restriction,
however, on anyone `deopping' themselves (using "-o").  Numeric
Replies:

		OK ERR_NEEDMOREPARAMS              OK RPL_CHANNELMODEIS
		OK ERR_CHANOPRIVSNEEDED            
		OK ERR_NOTONCHANNEL                ERR_KEYSET
		RPL_BANLIST                     RPL_ENDOFBANLIST
		OK ERR_UNKNOWNMODE                 OK ERR_NOSUCHCHANNEL

		ERR_USERSDONTMATCH              RPL_UMODEIS
		ERR_UMODEUNKNOWNFLAG



MODE - Changer le mode du channel :
— i : Définir/supprimer le canal sur invitation uniquement
6
ft_irc Internet Relay Chat
— t : Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
— k : Définir/supprimer la clé du canal (mot de passe)
— o : Donner/retirer le privilège de l’opérateur de canal
— l : Définir/supprimer la limite d’utilisateurs pour le canal
*/

void handleModeCommand(const char* message, Users *sender, Server *server)
{
	std::string modeMessage(message);
	std::istringstream iss(modeMessage);
	std::string command, target, mode, param;

	if (!(iss >> command >> target >> mode))
	{
		send(sender->getSocket(), ERR_NEEDMOREPARAMS(sender->getNickname(), command).c_str(),
			ERR_NEEDMOREPARAMS(sender->getNickname(), command).length(), 0);
		std::cout << "Error: not enough parameters" << std::endl;
		return;
	}

	if (target.size() > 0 && target[0] == '#')
	{
		Channels* channel = server->getChannelByName(target);
		if (channel == NULL)
		{
			send(sender->getSocket(), ERR_NOSUCHCHANNEL(sender->getNickname(), target).c_str(),
				ERR_NOSUCHCHANNEL(sender->getNickname(), target).length(), 0);
			std::cout << "Error: channel not found" << std::endl;
			return;
		}
		if (channel->getUserByName(sender->getNickname()) == NULL)
		{
			send(sender->getSocket(), ERR_NOTONCHANNEL(sender->getNickname(), target).c_str(),
				ERR_NOTONCHANNEL(sender->getNickname(), target).length(), 0);
			std::cout << "Error: user not on channel" << std::endl;
			return;
		}
		if (channel->getOperator(sender) == NULL)
		{
			send(sender->getSocket(), ERR_CHANOPRIVSNEEDED(sender->getNickname(), target).c_str(),
				ERR_CHANOPRIVSNEEDED(sender->getNickname(), target).length(), 0);
			std::cout << "Error: user is not a channel operator" << std::endl;
			return;
		}

		bool adding = true;
		for (std::string::size_type i = 0; i < mode.size(); ++i)
		{
			char c = mode[i];
			switch (c)
			{
				case '+':
					adding = true;
					break;
				case '-':
					adding = false;
					break;
				case 'i': // Invitation-only mode
					channel->setInviteOnly(adding);
					break;
				case 't': // Topic restrictions mode
					channel->setTopicRestrictions(adding);
					break;
				case 'k': // Channel key (password)
					if (adding)
					{
						if (iss >> param)
						{
							channel->setChannelKey(param);
						}
					}
					else
					{
						channel->removeChannelKey();
					}
					break;
				case 'o': // Operator privilege
					if (iss >> param)
					{
						if (adding)
						{
							std::cout << "Adding operator " << param << std::endl;
							channel->setOperator(param, *server);
						}
						else
						{
							channel->removeOperator(param);
						}
					}
					break;
				case 'l': // User limit
					if (adding)
					{
						if (iss >> param)
						{
							channel->setUserLimit(std::atoi(param.c_str()));
						}
					}
					else
					{
						channel->removeUserLimit();
					}
					break;
				default:
					send(sender->getSocket(), ERR_UNKNOWNMODE(sender->getNickname(), mode).c_str(),
						ERR_UNKNOWNMODE(sender->getNickname(), mode).length(), 0);
					break;
			}
		}

		// Send mode change confirmation to channel
		std::string confirmationMsg = "MODE " + target + " " + mode + " " + param + "\r\n";
		channel->broadcastMessage(confirmationMsg, *sender);
		send(sender->getSocket(), RPL_CHANNELMODEIS(sender->getNickname(), target, mode).c_str(),
			RPL_CHANNELMODEIS(sender->getNickname(), target, mode).length(), 0);
	}
}
