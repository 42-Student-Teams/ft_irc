/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:03:52 by inaranjo          #+#    #+#             */
/*   Updated: 2024/05/03 02:42:25 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Channel.hpp"

class Client
{
	private:
		int							_fd;
		bool						_isRegistred;
		bool						_isLogin;
		bool            			_passwordAuthenticated;
		std::string					_nickName;
		std::string 				_userName;
		std::string 				_buffer;
		std::string 				_clientIP;
		std::vector<std::string>	_ChannelsInvite;
	public:
		Client();
		Client(std::string nickname, std::string username, int fd);
		~Client();
		Client(Client const &src);
		Client &operator=(Client const &src);
		
		
		int						getFD();
		bool					getRegistered();
		std::string				getNickName();
		bool					getLogedIn();
		std::string 			getUserName();
		std::string 			getClientIP();
		std::string 			getBuffer();
		std::string 			getHostname();
		
		
		void					setFD(int fd);
		void					setNickName(std::string& nickName);
		void					setLogedin(bool value);
		void					setUserName(std::string& username);
		void					setBuffer(std::string recived);
		void					setRegistered(bool value);
		void					setClientIP(std::string ipadd);
		void            		setPasswordAuthenticated(bool authenticated);


		void					clearBuffer();
		bool					isChannelInvited(std::string &ChName);
		bool            		isPasswordAuthenticated() const;
		void					storeChannelInvite(std::string &chname);
		void					rmInvitationChan(std::string &chname);
		void 					write(const std::string& message);
};

#endif