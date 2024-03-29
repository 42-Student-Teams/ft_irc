/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Probook <Probook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:58:52 by inaranjo          #+#    #+#             */
/*   Updated: 2024/03/29 14:21:29 by Probook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Command::Command(Server* srv, bool auth) :_srv(srv), _auth(auth) {}

Command::~Command() {}

bool    Command::cmdPermission() const { return _auth; }