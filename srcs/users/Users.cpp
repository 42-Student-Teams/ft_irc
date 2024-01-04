/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Users.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:19:06 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/04 11:50:15 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"

Users::Users(void)
{
}

Users::Users(const std::string &nickname) : _nickname(nickname)
{
}

Users::Users(const int &socket) : _nickname("default"), _socket(socket)
{
}

Users::~Users(void)
{
}

int	Users::getSocket() const
{
	return (_socket);
}

std::string	Users::getNickname() const
{
	return (_nickname);
}
