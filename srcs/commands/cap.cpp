/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 12:28:35 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/24 14:32:51 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Users.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channels.hpp"

void handleCapCommand(const char* message, Users *sender, Server *server)
{
	(void) server;
	std::vector<std::string> tokens;
	char* token = std::strtok(const_cast<char*>(message), " ");
	while (token != nullptr)
	{
		tokens.push_back(token);
		token = std::strtok(nullptr, " ");
	}
	if (tokens.size() < 2)
	{
		// Handle error: insufficient arguments
		return;
	}
	const std::string& subcommand = tokens[1];
	if (subcommand == "LS")
	{
		std::string response = "CAP * LS :\r\n";
		send(sender->getSocket(), response.c_str(), response.length(), 0);
	}
}
