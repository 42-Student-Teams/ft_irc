/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Probook <Probook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 11:22:15 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/17 11:58:12 by Probook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Pass::Pass(Server* srv, bool auth) : Command(srv, auth) {}

Pass::~Pass() {}

// syntax: PASS <password>

void    Pass::execute(Client* client, std::vector<std::string> args)
{
    if (args.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PASS"));
        return;
    }

    if (client->registrationCheck())
    {
        client->reply(ERR_ALREADYREGISTERED(client->getNickname()));
        return;
    }

    if (_srv->getPassword() != args[0].substr(args[0][0] == ':' ? 1 : 0))
    {
        client->reply(ERR_PASSREJECT(client->getNickname()));
        return;
    }

    client->setState(LOGIN);
    client->reply("PASSWORD accepted. Please provide your USER information.\n");
}