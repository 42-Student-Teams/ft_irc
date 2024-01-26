/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:19:14 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/26 15:42:01 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channels.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/replies.hpp"

Channels::Channels(const std::string &name) : _name(name), _topic("default"),
												_inviteOnly(false), _topicRestrictions(false),
												_channelKey("default"), _userLimit(0)
{
}

void Channels::addUser(Users *user)
{
	if (!user)
		return;

	if ((int)_users.size() >= _userLimit && _userLimit != 0)
	{
		int fd = user->getFd().fd;
		send(fd, ERR_CHANNELISFULL(user->getNickname(), _name).c_str(),
			ERR_CHANNELISFULL(user->getNickname(), _name).size() + 1, 0);
		std::cout << "Error: channel is full" << std::endl;
		return;
	}

	for (std::list<Users*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (*it == user)
		return;
	}
	_users.push_back(user);
	std::string joinMessage = " joined the channel\n";
	std::string welcomeMessage = GREEN + joinMessage + RESET;
	broadcastMessage(welcomeMessage, *user);
}

void Channels::removeUser(Users *user)
{
	std::string partMessage = " left the channel\n";
	std::string quitMessage = RED + partMessage + RESET;
	broadcastMessage(quitMessage, *user);
	_users.remove(user);
}

void Channels::broadcastMessage(const std::string &message, Users &sender)
{
	std::string formattedMessage = ":" + sender.getNickname() + " PRIVMSG " + getName() + " :" + message + "\r\n";

	if (getUserByName(sender.getNickname()) == NULL)
	{
		std::cout << "Error: user not found in channel" << std::endl;
		return;
	}
	for (std::list<Users*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		Users* user = *it;
		if (user != &sender && getUserByName(user->getNickname()))
		{
			int fd = user->getFd().fd;
			send(fd, formattedMessage.c_str(), formattedMessage.size(), 0);
		}
	}
}

const std::string &Channels::getName() const
{
	return (_name);
}

Users	*Channels::getUserByName(std::string name) const
{
	for (std::list<Users*>::const_iterator it = _users.begin(); it != _users.end(); ++it)
	{
		Users* user = *it;
		if (user->getNickname() == name)
		{
			return (user);
		}
	}
	return (NULL);
}

void Channels::setTopic(const std::string &topic)
{
	_topic = topic;
}

const std::string &Channels::getTopic() const
{
	return (_topic);
}

const Users *Channels::getOperator(const Users *chanOperator) const
{
	for (std::list<Users*>::const_iterator it = _channel_operators.begin(); it != _channel_operators.end(); ++it)
	{
		Users* user = *it;
		if (user && chanOperator)
		{
			if (user->getNickname() == chanOperator->getNickname())
			{
				return (user);
			}
		}
	}
	return (NULL);
}

void Channels::setOperator(const std::string operatorName, Server &server)
{
	// _channel_operators.push_back(getUserByName(operatorName));
	// for (std::list<Users*>::iterator it = _channel_operators.begin(); it != _channel_operators.end(); ++it)
	// {
	// 	Users* user = *it;
	// 	std::cout << "Channel operator: " << user->getNickname() << std::endl;
	// }
	std::string	opname = operatorName;
	Users* user = server.getUserByNickname(opname);
	if (!user)
	{
		std::cout << "Error: user not found" << std::endl;
		return;
	}

	for (std::list<Users*>::iterator it = _channel_operators.begin(); it != _channel_operators.end(); ++it)
	{
		if (*it == user)
		{
			std::cout << "Error: user is already a channel operator" << std::endl;
			return;
		}
	}
	_channel_operators.push_back(user);
	for (std::list<Users*>::iterator it = _channel_operators.begin(); it != _channel_operators.end(); ++it)
	{
		Users* user = *it;
		std::cout << "Channel operator: " << user->getNickname() << std::endl;
	}
}

void Channels::removeOperator(const std::string operatorName)
{
	_channel_operators.remove(getUserByName(operatorName));
}

void Channels::setInviteOnly(bool inviteOnly)
{
	_inviteOnly = inviteOnly;
}

void Channels::setTopicRestrictions(bool topicRestrictions)
{
	_topicRestrictions = topicRestrictions;
}

void Channels::setChannelKey(const std::string &channelKey)
{
	_channelKey = channelKey;
}

void Channels::removeChannelKey()
{
	_channelKey = "";
}

void Channels::setUserLimit(int userLimit)
{
	_userLimit = userLimit;
}

void Channels::removeUserLimit()
{
	_userLimit = 0;
}
