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

int main(int argc, char **argv)
{
    if (argc != 3)
        throw std::runtime_error("Usage: ./ircserv <port> <password>");
    
    Server  server(argv[1], argv[2]);

    try
    {
        server.run();
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
