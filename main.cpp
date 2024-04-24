/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 20:20:20 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/19 20:21:21 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/Server.hpp"

bool isPortValid(std::string port){
	return (port.find_first_not_of("0123456789") == std::string::npos && \
	std::atoi(port.c_str()) >= 1024 && std::atoi(port.c_str()) <= 65535);
}

int main(int ac, char **av)
{
	Server ser;
	if (ac != 3)
		{std::cout << "Usage: " << av[0] << " <port number> <password>" << std::endl; return 1;}
	std::cout << "---- SERVER ----" << std::endl;
	try
	{
		signal(SIGINT, Server::handleSignal);
		signal(SIGQUIT, Server::handleSignal);
		if(!isPortValid(av[1]) || !*av[2] || std::strlen(av[2]) > 20)
			{std::cout << "invalid Port number / Password!" << std::endl; return 1;}
		ser.run(std::atoi(av[1]), av[2]);
	}
	catch(const std::exception& e)
	{
		ser.rmFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}
