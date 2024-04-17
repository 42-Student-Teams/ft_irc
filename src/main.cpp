/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 12:30:49 by inaranjo          #+#    #+#             */
/*   Updated: 2024/03/24 12:30:53 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scc/Server.hpp"

static bool checkPort (std::string port)
{
    for (size_t i = 0; i < port.size(); ++i) {
        if (port[i] < '0' || port[i] > '9') {
            return false;
        }
    }
    int portNumber = atoi(port.c_str());
    return (portNumber >= 1024 && portNumber <= 65535);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Error: Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    if (!checkPort(argv[1]) || strlen(argv[2]) > 20 || !*argv[2])
    {
        std::cout << "Error: Invalid port number or password" << std::endl;
        return 1;
    }

    Server server(argv[1], argv[2]);

    try
    {
        server.run();
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
