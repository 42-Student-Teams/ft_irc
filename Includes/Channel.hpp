/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:03:55 by inaranjo          #+#    #+#             */
/*   Updated: 2024/05/02 23:20:40 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Server.hpp"

class Client;
class Server;
class Channel
{
private:
	int _topic;
	int _key;
	int _maxClients;
	bool _isTopicRestricted;
	bool _inviteOnly;
	std::string _name;
	std::string _password;
	std::string _topicName;
	std::vector<Client> _clients;
	Server &_srv;
	std::vector<Client> _admins;

public:
	Channel();
	Channel(Server &server);
	~Channel();
	Channel(Channel const &src);
	Channel &operator=(Channel const &src);

	void setInvitation(int _VIP);
	void setTopic(int topic);
	void setKey(int key);
	void setMaxUsers(int limit);
	void setTopicName(std::string topic_name);
	void setPass(std::string password);
	void setName(std::string name);
	void setTime(std::string time);
	void setTopicRestric(bool value);
	void setModeAtindex(size_t index, bool mode);
	void setTimeCreation();

	int getVIP();
	bool getInviteOnly(); // new
	int getTopic();
	int getKey();
	int getMaxClients();
	int getNbClients();
	bool getTopicRestric() const;
	bool getModeAtindex(size_t index);
	std::string getTopicName();
	std::string getPass();
	std::string getName();
	std::string getTime();
	std::string getModes();
	std::string getChannelList();
	Client *getClientFd(int fd);
	Client *getAdminFd(int fd);
	Client *getClientInChannel(std::string name);
	size_t getSize() const;
	std::vector<Client *> getClients() const;

	bool isClientInChannel(int fd);
	bool isOperator(std::string const nickname);

	void setInviteOnly(bool flag);
	void setTopicControl(bool flag);
	void storeClient(Client newClient);
	void storeAdmin(Client newClient);
	void rmClientFd(int fd);
	void rmAdminFd(int fd);

	void sendMsgToAll(std::string rpl1);
	void sendMsgToAll(std::string rpl1, int fd);
	void addOperator(Client *client);
	void removeOperator(Client *client);
	void changeOperatorStatus(Client *client, const std::string &targetNick, bool adding);
	void broadcastModeChange(const std::string &prefix, const std::string &modes, const std::string &param);
	void addClient(Client *client);
	void removeClient(int fd);
};

#endif