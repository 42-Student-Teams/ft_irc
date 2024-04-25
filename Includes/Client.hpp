/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Probook <Probook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:03:52 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/25 02:37:25 by Probook          ###   ########.fr       */
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
		//bool						_isOperator;
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
		std::string 			getInfo() const;
		
		
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