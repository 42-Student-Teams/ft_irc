/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:03:52 by inaranjo          #+#    #+#             */
/*   Updated: 2024/05/03 11:56:21 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Channel.hpp"

class Client
{
private:
	int _fd;
	bool _isRegistered;
	bool _isLogin;
	bool _isNickSet;
	bool _bigWelcome;
	std::string _nickName;
	std::string _password;
	std::string _userName;
	std::string _buffer;
	
	std::string _clientIP;
	std::vector<std::string> _ChannelsInvite;

public:
	Client();
	Client(std::string nickname, std::string username, int fd);
	~Client();
	Client(Client const &src);
	Client &operator=(Client const &src);

	int getFD();
	bool getRegistered();
	std::string getNickName();
	bool getLogedIn();
	std::string getPassword();
	bool getNickReceived(void);
	bool BigWelcomeSent (void);

	std::string getUserName();
	std::string getClientIP();
	std::string getBuffer();
	std::string getHostname();

	void setFD(int fd);
	void setNickName(std::string &nickName);
	void setLogedin(bool value);
	void setUserName(std::string &username);
	void setBuffer(std::string recived);
	void setRegistered(bool value);
	void setClientIP(std::string ipadd);
	void setReceivedInfo(bool value);
	void setBigWelcomeSent(bool value);
	void setPassword(std::string &password);

	void clearBuffer();
	bool isChannelInvited(std::string &ChName);
	bool isPasswordAuthenticated() const;
	void storeChannelInvite(std::string &chname);
	void rmInvitationChan(std::string &chname);
	void write(const std::string &message);
};

#endif