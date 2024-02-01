/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Users.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:19:06 by ndiamant          #+#    #+#             */
/*   Updated: 2024/02/01 15:34:43 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"

Users::Users(const int &socket) : _nickname("default"), _username("default"), _realname("default"),
	_socket(socket), _isOperator(false), _isRegistered(false)
{
	_fd.fd = -1;
	_fd.events = 0;
	_fd.revents = 0;
}

Users::~Users(void)
{
	_isRegistered = false;
}

int	Users::getSocket() const
{
	return (_socket);
}

std::string	Users::getNickname() const
{
	return (_nickname);
}

struct pollfd	Users::getFd() const
{
	return (_fd);
}

Channels**	Users::getCurrentChannels() const
{
	if (!_currentChannels.top())
		return (NULL);
	return (_currentChannels);
}

Channels*	Users::getChannelByName(const std::string &channelName) const;
{
	for (std::vector<Channels*>::iterator it = _currentChannels.begin(); it != _currentChannels.end(); ++it)
	{
		if (*it->getName() == channelName)
			return (*it);
	}
	return (NULL);
}

void	Users::setFd(struct pollfd fd)
{
		_fd = fd;
}

void	Users::setNickname(const std::string &nickname)
{
	_nickname = nickname;
}

void	Users::setCurrentChannel(Channels* channel)
{
	for (std::vector<Channels*>::iterator it = _currentChannels.begin(); it != _currentChannels.end(); ++it)
	{
		if (*it == channel)
			return;
	}
	_currentChannels.push_back(channel);
}

bool	Users::isOperator() const
{
	return (_isOperator);
}

void	Users::setOperator(bool isOperator)
{
	_isOperator = isOperator;
}

bool	Users::isRegistered() const
{
	return (_isRegistered);
}

bool	Users::setRegistered(bool isRegistered)
{
	_isRegistered = isRegistered;
	return (_isRegistered);
}

void	Users::setUsername(const std::string &username)
{
	_username = username;
}

void	Users::setRealname(const std::string &realname)
{
	_realname = realname;
}

std::string	Users::getUsername() const
{
	return (_username);
}

std::string	Users::getRealname() const
{
	return (_realname);
}
