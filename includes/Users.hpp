/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Users.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:18:30 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/04 11:50:32 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERS_HPP

# define USERS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#pragma once

class Users
{
	private:
		std::string _nickname;
		int			_socket;
	
	public:
		Users(void);
		Users(const std::string &nickname);
		Users(const int &socket);
		~Users(void);

		int	getSocket() const;
		std::string	getNickname() const;

};

#endif
