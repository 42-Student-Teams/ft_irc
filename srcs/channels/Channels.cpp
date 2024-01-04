/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:19:14 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/04 11:52:37 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channels.hpp"

Channels::Channels(const std::string &name) : _name(name)
{
}

void Channels::addUser(Users* user)
{
	_users.push_back(user);
}

void Channels::removeUser(Users* user)
{
	_users.erase(std::remove(_users.begin(), _users.end(), user), _users.end());
}

void Channels::broadcastMessage(const std::string &message, Users &sender)
{
	for(std::vector<Users*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		Users* user = *it;
		if(user != &sender)
			std::cout << "Message to " << user->getNickname() << ": " << message << std::endl;
	}
}

const std::string &Channels::getName() const
{
	return (_name);
}
