/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:19:14 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/24 18:53:23 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channels.hpp"

Channels::Channels(const std::string &name) : _name(name), _topic("default")
{
}

void Channels::addUser(Users *user)
{
	if (!user)
		return;

	for (std::list<Users*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (*it == user)
		return;
	}
	_users.push_back(user);
	std::string welcomeMessage = GREEN + user->getNickname() + " joined the channel\n" + RESET;
	for (std::list<Users*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		Users* user = *it;
		{
			int fd = user->getFd().fd;
			send(fd, welcomeMessage.c_str(), welcomeMessage.size() + 1, 0);
		}
	}
}

void Channels::removeUser(Users *user)
{
	std::string quitMessage = RED + user->getNickname() + " left the channel\n" + RESET;
	for (std::list<Users*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		Users* user = *it;
		{
			int fd = user->getFd().fd;
			send(fd, quitMessage.c_str(), quitMessage.size() + 1, 0);
		}
	}
	_users.remove(user);
}

void Channels::broadcastMessage(const std::string &message, Users &sender)
{
	std::string formattedMessage = ":" + sender.getNickname() + " PRIVMSG " + getName() + " :" + message + "\r\n";

	for (std::list<Users*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		Users* user = *it;
		if (user != &sender)
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

const Users *Channels::getOperator() const
{
	return (_channel_operator);
}

void Channels::setOperator(const Users *operatorName)
{
	_channel_operator = const_cast<Users *>(operatorName);
}
