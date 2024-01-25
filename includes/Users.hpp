/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Users.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:18:30 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/24 18:39:02 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERS_HPP

# define USERS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <poll.h>
#include "Channels.hpp"

#pragma once

class Channels;

class Users
{
	private:
		std::string 		_nickname;
		std::string			_username;
		std::string			_realname;
		int					_socket;
		struct pollfd		_fd;
		Channels*			_currentChannel;
		bool				_isOperator;
		bool				_isRegistered;
		std::string			_mode;

	
	public:
		Users(const int &socket);
		~Users(void);

		int	getSocket() const;
		std::string		getNickname() const;
		std::string		getUsername() const;
		std::string		getRealname() const;
		struct pollfd	getFd() const;
		Channels*		getCurrentChannel() const;

		void			setFd(struct pollfd fd);
		void			setNickname(const std::string &nickname);
		void			setCurrentChannel(Channels* channel);
		void			setUsername(const std::string &username);
		void			setRealname(const std::string &realname);

		bool			isOperator() const;
		void			setOperator(bool isOperator);
		bool			isRegistered() const;
		bool			setRegistered(bool isRegistered);

};

#endif
