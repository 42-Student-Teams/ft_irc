/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:58:52 by inaranjo          #+#    #+#             */
/*   Updated: 2024/03/24 11:41:41 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Command::Command(Server* srv, bool auth) :_srv(srv), _auth(auth) {}

Command::~Command() {}

bool    Command::cmdPermission() const { return _auth; }