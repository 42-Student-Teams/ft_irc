/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:03:55 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/19 17:20:06 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Server.hpp"

class Client;
class Channel
{
	private:
		int									_VIP;
		int 								_topic;
		int 								_key;
		int									_maxClients;
		bool								_isTopicRestricted;
		std::string 						_name;
		std::string							_time;
		std::string 						_password;
		std::string 						_created_at;
		std::string 						_topicName;
		std::vector<Client> 				_clients;
		std::vector<Client> 				_admins;
		std::vector<std::pair<char, bool> > _modes;
public:
	Channel();
	~Channel();
	Channel(Channel const &src);
	Channel &operator=(Channel const &src);
	

	void				setInvitation(int _VIP);
	void				setTopic(int topic);
	void				setKey(int key);
	void				setMaxUsers(int limit);
	void				setTopicName(std::string topic_name);
	void				setPass(std::string password);
	void				setName(std::string name);
	void				setTime(std::string time);
	void				setTopicRestric(bool value);
	void				setModeAtindex(size_t index, bool mode);
	void				setTimeCreation();


	int 				getVIP();
	int 				getTopic();
	int 				getKey();
	int 				getMaxClients();
	int 				getClientsNumber();
	bool 				getTopicRestric() const;
	bool 				getModeAtindex(size_t index);
	std::string			getTopicName();
	std::string			getPass();
	std::string 		getName();
	std::string 		getTime();
	std::string 		getTimeCreation();
	std::string 		getModes();
	std::string			getChannelList();
	Client				*getClientFd(int fd);
	Client 				*getAdminFd(int fd);
	Client 				*getClientInChannel(std::string name);


	bool isClientInChannel(std::string &nick);

	
	void storeClient(Client newClient);
	void storeAdmin(Client newClient);
	void rmClientFd(int fd);
	void rmAdminFd(int fd);
	bool clientTOadmin(std::string& nick);
	bool adminTOclient(std::string& nick);


	void sendMsgToAll(std::string rpl1);
	void sendMsgToAll(std::string rpl1, int fd);
};

#endif