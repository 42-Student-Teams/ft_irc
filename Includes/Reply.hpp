/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Probook <Probook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:04:00 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/25 12:44:15 by Probook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define once

#define CRLF "\r\n"

#define RPL_CONNECTED(nickname) (": 001 " + nickname + " : Welcome to the IRC server!" + CRLF)
#define RPL_UMODEIS(hostname, channelname, mode, user)  ":" + hostname + " MODE " + channelname + " " + mode + " " + user + CRLF
#define RPL_CREATIONTIME(nickname, channelname, creationtime) ": 329 " + nickname + " #" + channelname + " " + creationtime + CRLF
#define RPL_CHANNELMODES(nickname, channelname, modes) ": 324 " + nickname + " #" + channelname + " " + modes + CRLF
#define RPL_CHANGEMODE(hostname, channelname, mode, arguments) (":" + hostname + " MODE #" + channelname + " " + mode + " " + arguments + CRLF)
#define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + CRLF)
#define RPL_JOINMSG(hostname, ipaddress, channelname) (":" + hostname + "@" + ipaddress + " JOIN #" + channelname + CRLF)
#define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " @ #" + channelname + " :" + clientslist + CRLF)
#define RPL_ENDOFNAMES(nickname, channelname) (": 366 " + nickname + " #" + channelname + " :END of /NAMES list" + CRLF)
#define RPL_TOPICIS(nickname, channelname, topic) (": 332 " + nickname + " #" +channelname + " :" + topic + "\r\n")
#define RPL_PASSACCEPTED(nickname) (": 230 " + nickname + " :Password accepted - registration successful!" + CRLF)


///////// ERRORS ////////////////
#define ERR_NEEDMODEPARM(channelname, mode) (": 696 #" + channelname + " * You must specify a parameter for the key mode. " + mode + CRLF)
#define ERR_INVALIDMODEPARM(channelname, mode) ": 696 #" + channelname + " Invalid mode parameter. " + mode + CRLF
#define ERR_KEYSET(channelname) ": 467 #" + channelname + " Channel key already set. " + CRLF
#define ERR_UNKNOWNMODE(nickname, channelname, mode) ": 472 " + nickname + " #" + channelname + " " + mode + " :is not a recognised channel mode" + CRLF
#define ERR_NOTENOUGHPARAM(nickname) (": 461 " + nickname + " :Not enough parameters." + CRLF)
#define ERR_CHANNELNOTFOUND(nickname, channelname) (": 403 " + nickname + " " + channelname + " :No such channel" + CRLF)
#define ERR_NOTOPERATOR(channelname) (": 482 #" + channelname + " :You're not a channel operator" + CRLF)
#define ERR_NOSUCHNICK(channelname, name) (": 401 #" + channelname + " " + name + " :No such nick/channel" + CRLF )
#define ERR_INCORPASS(nickname) (": 464 " + nickname + " :Password incorrect !" + CRLF )
#define ERR_ALREADYREGISTERED(nickname) (": 462 " + nickname + " :You may not reregister !" + CRLF )
#define ERR_NONICKNAME(nickname) (": 431 " + nickname + " :No nickname given" + CRLF )
#define ERR_NICKINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + CRLF)
#define ERR_ERRONEUSNICK(nickname) (": 432 " + nickname + " :Erroneus nickname" + CRLF)
#define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + CRLF)
#define ERR_CMDNOTFOUND(nickname, command) (": 421 " + nickname + " " + command + " :Unknown command" + CRLF)


#define ERR_NOTREGISTERED2(nickname) (std::string(": 451 ")  + (nickname) + " :You have not registered!" + CRLF)
#define ERR_NONICKNAME2(nickname) (std::string(": 431 ") + (nickname) + " :No nickname given" + CRLF)
#define ERR_NOTENOUGHPARAM2(nickname) (std::string(": 461 ") + std::string(nickname) + " :Not enough parameters." + std::string(CRLF))
#define ERR_INCORPASS2(nickname) (std::string(": 464 ") + std::string(nickname) + " :Password incorrect !" + std::string(CRLF))

#define ERR_NICKNAMEINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + CRLF)
#define ERR_NICKCOLLISION(nickname) (": 436 " + nickname + " :Nickname collision KILL sent" + CRLF)

/* COMMANDE */
#define ERR_PASSREJECT(source)                           "464 " + source + " :Password is incorrect" + CRLF
#define ERR_CHANOPRIVSNEEDED(source, channel)           "482 " + source + " " + channel + " :You're not channel operator" + CRLF
#define ERR_NOORIGIN(source)                            "409 " + source + " :No origin specified" + CRLF
#define ERR_NOSUCHCHANNEL(source, channel)              "403 " + source + " " + channel + " :No such channel" + CRLF
#define ERR_NOTONCHANNEL(source, channel)               "442 " + source + " " + channel + " :You're not on that channel" + CRLF
#define RPL_LIST(nickname, channel, users, topic)       ("322 " + std::string(nickname) + " " + std::string(channel) + " " + std::to_string(users) + " :" + std::string(topic) + CRLF) 
#define RPL_LISTEND(nickname)                           "323 " + nickname + " :End of LIST" + CRLF
#define ERR_NOTENOUGHPARAMS(source, command)            "461 " + source + " " + command + " :Not enough parameters" + CRLF
#define ERR_CANNOTSENDTOCHAN(source, channel)           "404 " + source + " " + channel + " :Cannot send to channel" + CRLF
#define RPL_NOTOPIC(source, channel)                    "331 " + source + " " + channel + " :No topic is set" + CRLF
#define RPL_TOPIC(source, channel, topic)               "332 " + source + " " + channel + " :" + topic + CRLF
#define RPL_WHOREPLY(source, channel, user_info)        "352 " + source + " " + channel + " " + user_info + CRLF
#define RPL_ENDOFWHO(nickname)                          "366 " + nickname + " :End of WHO list" + CRLF
#define ERR_BADCHANNELKEY(source, channel)              "475 " + source + " " + channel + " :Cannot join channel (+k)" + CRLF
#define ERR_NEEDMOREPARAMS(source, command)             ("461 " + std::string(source) + " " + std::string(command) + " :Not enough parameters" + CRLF)
