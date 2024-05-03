/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 19:45:41 by inaranjo          #+#    #+#             */
/*   Updated: 2024/05/03 13:09:32 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Client.hpp"

Client::Client() : _fd(-1),
				   _isRegistered(false),
				   _isLogin(false),
				   _isNickSet(false),
				   _bigWelcome(false),
				   _nickName(""),
				   _password(""),
				   _userName(""),
				   _buffer(""),
				   _clientIP("")
{
}

Client::Client(std::string nickname, std::string username, int fd) : _fd(fd),
																	 _nickName(nickname),
																	 _userName(username)
{
}

Client::~Client() {}

Client::Client(Client const &src) { *this = src; }

Client &Client::operator=(Client const &src)
{
	if (this != &src)
	{
		_nickName = src._nickName;
		_userName = src._userName;
		_fd = src._fd;
		_bigWelcome = src._bigWelcome;
		_isNickSet = src._isNickSet;
		_password = src._password;
		_ChannelsInvite = src._ChannelsInvite;
		_buffer = src._buffer;
		_isRegistered = src._isRegistered;
		_clientIP = src._clientIP;
		_isLogin = src._isLogin;
	}
	return *this;
}

int Client::getFD() { return _fd; }
bool Client::getRegistered()
{
	return this->_isRegistered;
}

bool Client::getLogedIn() { return _isLogin; }
std::string Client::getNickName() { return _nickName; }

bool Client::getNickReceived() { return _isNickSet; }

std::string Client::getUserName() { return _userName; }
bool Client::BigWelcomeSent() { return _bigWelcome; }
std::string Client::getBuffer() { return _buffer; }
std::string Client::getClientIP() { return _clientIP; }
std::string Client::getHostname()
{
	std::string hostname = getNickName() + "!" + getUserName();
	return hostname;
}

void Client::setPassword(std::string &password) { _password = password; }
void Client::setFD(int fd) { _fd = fd; }
void Client::setNickName(std::string &nickName) { _nickName = nickName; }
void Client::setLogedin(bool value) { _isLogin = value; }
void Client::setUserName(std::string &username) { _userName = username; }
void Client::setBuffer(std::string recived) { _buffer += recived; }
void Client::setRegistered(bool value) { _isRegistered = value; }
void Client::setClientIP(std::string ipadd) { _clientIP = ipadd; }

std::string Client::getPassword() { return _password; }
void Client::setReceivedInfo(bool value) { _isNickSet = value; }
void Client::setBigWelcomeSent(bool value) { _bigWelcome = value; }

void Client::clearBuffer() { _buffer.clear(); }
bool Client::isChannelInvited(std::string &ChName)
{
	for (size_t i = 0; i < _ChannelsInvite.size(); i++)
	{
		if (_ChannelsInvite[i] == ChName)
			return true;
	}
	return false;
}
void Client::storeChannelInvite(std::string &chname)
{
	_ChannelsInvite.push_back(chname);
}
void Client::rmInvitationChan(std::string &chname)
{
	for (size_t i = 0; i < _ChannelsInvite.size(); i++)
	{
		if (_ChannelsInvite[i] == chname)
		{
			_ChannelsInvite.erase(_ChannelsInvite.begin() + i);
			return;
		}
	}
}

void Client::write(const std::string &message)
{
	if (send(_fd, message.c_str(), message.size(), 0) == -1)
	{
		std::cerr << "Failed to send message to client: " << strerror(errno) << std::endl;
	}
}
