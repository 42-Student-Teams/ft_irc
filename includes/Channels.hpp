/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:16:43 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/24 18:53:01 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELS_HPP

# define CHANNELS_HPP

#pragma once

#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE	"\033[0;34m"
#define MAG		"\033[0;35m"
#define CYN 	"\033[0;36m"

#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include <sys/socket.h>
#include "Users.hpp"

class Users;

class Channels
{
private:
	std::string 			_name;
	std::list<Users *>		_users;
	Users*					_channel_operator;
	std::string 			_topic;
	std::string				_mode;

public:
	Channels(const std::string &name);

	void addUser(Users *user);
	void removeUser(Users *user);
	void broadcastMessage(const std::string &message, Users &sender);

	const std::string& getName() const;
	Users *getUserByName(std::string name) const;
	void setTopic(const std::string &topic);
	const std::string& getTopic() const;
	const Users *getOperator() const;
	void setOperator(const Users *operatorName);
};

#endif
