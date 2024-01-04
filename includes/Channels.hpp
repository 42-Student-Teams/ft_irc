/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:16:43 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/04 11:45:30 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELS_HPP

# define CHANNELS_HPP

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Users.hpp"

class Users;

class Channels
{
	private:
		std::string _name;
		std::vector<Users*> _users;
	
	public:
		Channels(const std::string &name);

		void addUser(Users* user);
		void removeUser(Users* user);
		void broadcastMessage(const std::string &message, Users &sender);

		const std::string& getName() const;
};

#endif
