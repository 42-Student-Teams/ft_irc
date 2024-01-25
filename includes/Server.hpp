/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndiamant <ndiamant@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:28:27 by ndiamant          #+#    #+#             */
/*   Updated: 2024/01/24 18:54:11 by ndiamant         ###   ########.fr       */
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
#include <list>
#include <cstdlib>
#include <ctime>
#include <sstream>
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
		std::list<struct pollfd>	_fds;
		std::list<Channels>			_channels;
		std::list<Users>			_users;

	public:

		Server(int port, std::string password);
		~Server(void);

		bool	setup(void);
		void	ensureChannelExists(const std::string& channelName, Users *creator);
		void	handleNewConnection(int client_socket);
		void	handleMessage(int userIndex, const char* message);
		void	run(void);
		void	broadcastMessage(const char* message, int except_fd);
		//void	sendServerRpl(int const client_fd, std::string client_buffer);
		
		
		Channels*	getChannelByName(const std::string& name);
		std::string	getPassword(void);
		Users*		getUserByNickname(std::string& nickname);
};

#endif
