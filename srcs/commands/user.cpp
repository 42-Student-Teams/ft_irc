/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:57:28 by ndiamant          #+#    #+#             */
/*   Updated: 2024/02/08 14:38:28 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"
#include "../../includes/replies.hpp"
#include <ctime>
#include <iomanip>

void handleQuitCommand(const char* message, Users* sender, Server* server);

/*
      Command: USER
   Parameters: <username> <hostname> <servername> <realname>
   The USER message is used at the beginning of connection to specify
   the username, hostname, servername and realname of s new user.  It is
   also used in communication between servers to indicate new user
   arriving on IRC, since only after both USER and NICK have been
   received from a client does a user become registered.
   Between servers USER must to be prefixed with client's NICKname.
   Note that hostname and servername are normally ignored by the IRC
   server when the USER command comes from a directly connected client
   (for security reasons), but they are used in server to server
   communication.  This means that a NICK must always be sent to a
   remote server when a new user is being introduced to the rest of the
   network before the accompanying USER is sent.
   It must be noted that realname parameter must be the last parameter,
   because it may contain space characters and must be prefixed with a
   colon (':') to make sure this is recognised as such.
   Since it is easy for a client to lie about its username by relying
   solely on the USER message, the use of an "Identity Server" is
   recommended.  If the host which a user connects from has such a
   server enabled the username is set to that as in the reply from the
   "Identity Server".
   Numeric Replies:
           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
*/

std::string getCurrentDate()
{
	std::time_t now = std::time(0);
	
	std::tm* now_tm = std::localtime(&now);
	
	std::stringstream ss;
	ss << (now_tm->tm_year + 1900) << '-' 
	<< std::setw(2) << std::setfill('0') << (now_tm->tm_mon + 1) << '-'
	<< std::setw(2) << std::setfill('0') << now_tm->tm_mday;

	return ss.str();
}

void handleUserCommand(const char* message, Users *sender, Server *server)
{
	(void) server;
	if (sender->getHasEnteredPassword() == false)
	{
		std::string errMsg = "ERROR :You must enter a password first\r\n";
		send(sender->getSocket(), errMsg.c_str(), errMsg.size(), 0);
		handleQuitCommand("QUIT", sender, server);
		return;
	}
	if (sender->isRegistered())
	{
		// Send error message for already registered users
		std::string errMsg = "ERROR :You are already registered\r\n";
		send(sender->getSocket(), errMsg.c_str(), errMsg.size(), 0);
		return;
	}

	std::string msg(message);

	// Expected format: USER <username> <hostname> <servername> :<realname>
	std::istringstream iss(msg.substr(5)); // Skip "USER "
	std::string username, hostname, servername, realname;
	iss >> username >> hostname >> servername;

	// Extract realname (which may contain spaces and starts with a colon)
	std::size_t colonPos = msg.find(" :", iss.tellg());
	if (colonPos != std::string::npos) {
		realname = msg.substr(colonPos + 2); // Skip " :"
	}

	// Check if all required parameters were provided
	if (username.empty() || hostname.empty() || servername.empty() || realname.empty()) {
		std::string errMsg = "ERROR :Not enough parameters\r\n";
		send(sender->getSocket(), errMsg.c_str(), errMsg.size(), 0);
		return;
	}

	// Set user details
	sender->setUsername(username);
	sender->setRealname(realname);
	if (sender->isRegistered() == false && sender->getNickname() != "default" && sender->getUsername() != "default")
	{
		sender->setRegistered(true);
		std::cout << "User registered" << std::endl;
	}
	std::string toSend = RPL_WELCOME(user_id(sender->getNickname(), sender->getUsername()), sender->getNickname());
	send(sender->getSocket(), toSend.c_str(), toSend.length(), 0);
	toSend = RPL_YOURHOST(sender->getNickname(), "ft_irc", "1.1");
	send(sender->getSocket(), toSend.c_str(), toSend.length(), 0);
	toSend = RPL_CREATED(sender->getNickname(), getCurrentDate());
	send(sender->getSocket(), toSend.c_str(), toSend.length(), 0);
}
