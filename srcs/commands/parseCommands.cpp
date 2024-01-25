/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseCommands.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 12:00:56 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/24 14:34:38 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cstring>
#include "../../includes/Server.hpp"
#include "../../includes/Users.hpp"
#include "../../includes/replies.hpp"

void handleJoinCommand(const char* message, Users *sender, Server *server);
void handleNickCommand(const char* message, Users *sender, Server *server);
void handleOperCommand(const char* message, Users *sender, Server *server);
void handleKickCommand(const char* message, Users *sender, Server *server);
void handleInviteCommand(const char* message, Users *sender, Server *server);
void handlePartCommand(const char* message, Users *sender, Server *server);
void handleQuitCommand(const char* message, Users *sender, Server *server);
void handleTopicCommand(const char* message, Users *sender, Server *server);
void handleModeCommand(const char* message, Users *sender, Server *server);
void handleUserCommand(const char* message, Users *sender, Server *server);
void handlePassCommand(const char* message, Users *sender, Server *server);
void handleMessageCommand(const char* message, Users *sender, Server *server);
void handlePingCommand(const char* message, Users *sender, Server *server);
void handleNoticeCommand(const char* message, Users *sender, Server *server);
void handleCapCommand(const char* message, Users *sender, Server *server);

enum CommandType
{
	UNKNOWN,
	KICK,
	INVITE,
	TOPIC,
	MODE,
	JOIN,
	NICK,
	OPER,
	QUIT,
	PART,
	USER,
	PASS,
	PRIVMSG,
	PING,
	NOTICE,
	CAP
};

CommandType checkIrcCommand(const char* message)
{
	if (std::strncmp(message, "KICK", 4) == 0) return KICK;
	else if (std::strncmp(message, "JOIN", 4) == 0) return JOIN;
	else if (std::strncmp(message, "INVITE", 6) == 0) return INVITE;
	else if (std::strncmp(message, "TOPIC", 5) == 0) return TOPIC;
	else if (std::strncmp(message, "MODE", 4) == 0) return MODE;
	else if (std::strncmp(message, "NICK", 4) == 0) return NICK;
	else if (std::strncmp(message, "OPER", 4) == 0) return OPER;
	else if (std::strncmp(message, "QUIT", 4) == 0) return QUIT;
	else if (std::strncmp(message, "PART", 4) == 0) return PART;
	else if (std::strncmp(message, "USER", 4) == 0) return USER;
	else if (std::strncmp(message, "PASS", 4) == 0) return PASS;
	else if (std::strncmp(message, "PRIVMSG", 7) == 0) return PRIVMSG;
	else if (std::strncmp(message, "PING", 4) == 0) return PING;
	else if (std::strncmp(message, "NOTICE", 6) == 0) return NOTICE;
	else if (std::strncmp(message, "CAP", 3) == 0) return CAP;

	return UNKNOWN;
}

int parseCommands(const char* message, Users *sender, Server *server)
{
	CommandType cmdType = checkIrcCommand(message);

	switch(cmdType)
	{
		case JOIN:
			if (sender->isRegistered() == false)
			{
				send(sender->getSocket(), ERR_NOTREGISTERED(sender->getNickname()).c_str(), 
					ERR_NOTREGISTERED(sender->getNickname()).length(), 0);
			}
			else
			{
				handleJoinCommand(message, sender, server);
			}
			break;
		case NICK:
			handleNickCommand(message, sender, server);
			break;
		case OPER:
			handleOperCommand(message, sender, server);
			break;
		case QUIT:
			handleQuitCommand(message, sender, server);
			break;
		case KICK:
			handleKickCommand(message, sender, server);
			break;
		case PART:
			handlePartCommand(message, sender, server);
			break;
		case INVITE:
			handleInviteCommand(message, sender, server);
			break;
		case TOPIC:
			handleTopicCommand(message, sender, server);
			break;
		case MODE:
			handleModeCommand(message, sender, server);
			break;
		case USER:
			handleUserCommand(message, sender, server);
			break;
		case PASS:
			handlePassCommand(message, sender, server);
			break;
		case PRIVMSG:
			handleMessageCommand(message, sender, server);
			break;
		case PING:
			handlePingCommand(message, sender, server);
			break;
		case NOTICE:
			handleNoticeCommand(message, sender, server);
			break;
		case CAP:
			handleCapCommand(message, sender, server);
			break;
		default:
			return (0);
	}
	return (1);
}
