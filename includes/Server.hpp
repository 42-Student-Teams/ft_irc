/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:28:27 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/04 12:01:31 by ndiamant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP

# define SERVER_HPP

#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE	"\033[0;34m"
#define MAG		"\033[0;35m"
#define CYN 	"\033[0;36m"


#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include "Users.hpp"
#include "Channels.hpp"

#define TIMEOUT 3000

class Server
{
	private:
		int 						_server_fd;
		int 						_port;
		std::string 				_password;
		struct sockaddr_in			_address;
		int							_addrlen;
		std::vector<struct pollfd>	_fds;
		std::vector<Channels>		_channels;
		std::vector<Users>			_users;

	public:

		Server(int port, std::string password);
		~Server(void);

		bool	setup(void);
		void	ensureChannelExists(const std::string& channelName);
		void	handleNewConnection(int client_socket);
		void	handleMessage(int userIndex, const char* message);
		void	run(void);
		void	broadcastMessage(const char* message, int except_fd);
};

#endif
