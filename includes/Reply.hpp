/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Probook <Probook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 12:30:00 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/17 11:41:24 by Probook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef REPLY_HPP
#define REPLY_HPP

#include <iostream>
#include <string>

#include <cstdlib>
#include <cstdio>
#include <time.h>

#define YELLOW "\e[93;5;226m"
#define RESET  "\e[0m"
#define GREEN  "\e[92;5;118m"
#define VIOLET "\033[35m"
#define RED "\033[31m"
#define ORANGE "\033[38;5;208m"
#define BLUE "\033[34m"
#define LIGHT_BLUE "\033[36m"
#define LIGHT_CYAN "\033[96m"
#define LIGHT_PURPLE "\033[95m"
#define MAGENTA "\033[35m"
#define LIGHT_ORANGE "\033[38;5;208m"
#define GRAY "\033[90m"

/*Clients folder*/
#define RPL_WELCOME(source)                             "001 " + source + " : Welcome " + source + " to the IRC 42 Chat"
#define RPL_NAMREPLY(source, channel, users)            "353 " + source + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(source, channel)                 "366 " + source + " " + channel + " : End of /NAMES list."

#define RPL_JOIN(source, channel)                       ":" + source + " JOIN :" + channel
#define RPL_PART(source, channel)                       ":" + source + " PART :" + channel

/*Channel folder*/
#define RPL_KICK(source, channel, target, reason)       ":" + source + " KICK " + channel + " " + target + " :" + reason

/*Parser file*/
#define ERR_NOTREGISTERED(source)                       "451 " + source + " : You have not registered"
#define ERR_UNKNOWNCOMMAND(source, command)             "421 " + source + " " + command + " : Unknown command"

/*NICK file*/
#define ERR_NONICKNAMEGIVEN(source)                     "431 " + source + " :Nickname not given"
#define ERR_NICKNAMEINUSE(source)                       "433 " + source + " " + source  + " : Nickname is already in use"

/*QUIT file*/
#define RPL_QUIT(source, message)                       ":" + source + " QUIT :Quit: " + message

/*User file*/
#define ERR_ALREADYREGISTERED(source)                   "462 " + source + " :You may not register"
#define ERR_NEEDMOREPARAMS(source, command)             "461 " + source + " " + command + " : <username> <hostname> <servername> <realname>"

/*JOIN file*/
#define ERR_BADCHANNELKEY(source, channel)              "475 " + source + " " + channel + " :Cannot join channel (+k) - bad key"

/*LIST file*/
#define RPL_LISTEND(source)                             "323 " + source + " :End of LIST"
#define RPL_LIST(nickname, channelName, size, topic)    "322 " + nickname + " " + channelName + " " + std::to_string(size) + " :" + topic

/*PART file*/
#define ERR_NOSUCHCHANNEL(source, channel)              "403 " + source + " " + channel + " :No such channel"
#define ERR_NOTONCHANNEL(source, channel)               "442 " + source + " " + channel + " :You're not on that channel"

/*PRIVMSG file*/
#define ERR_NOTENOUGHPARAMS(source, command)            "461 " + source + " " + command + " :Not enough parameters"
#define ERR_CANNOTSENDTOCHAN(source, channel)           "404 " + source + " " + channel + " :Cannot send to channel"
#define ERR_NOSUCHNICK(source, nickname)                "401 " + source + " " + nickname + " :No such nick/channel"

/*TOPIC file*/
#define RPL_NOTOPIC(source, channel)                    "331 " + source + " " + channel + " :No topic is set"
#define RPL_TOPIC(source, channel, topic)               "332 " + source + " " + channel + " :" + topic
#define ERR_NOSUCHCHANNEL(source, channel)              "403 " + source + " " + channel + " :No such channel"

/*WHO file*/
#define RPL_ENDOFWHO(nickname)                          "366 " + nickname + " :End of WHO list."
#define RPL_WHOREPLY(source, channel, userInfo)         "352 " + source + " " + channel + " " + userInfo

/*PASS FILE*/
#define ERR_PASSWDMISMATCH(source)                      "464 " + source + " :Password incorrect"

/*MODE FILE*/
#define ERR_CHANOPRIVSNEEDED(source, channel)           "482 " + source + " " + channel + " :You're not channel operator"
#define ERR_UNKNOWNMODE(source, mode)                   "472 " + source + " " + mode + " :is unknown mode char to me"

static inline void serverON(const std::string& message) 
{
    time_t      rawtime;
    struct tm   *timeinfo;
    char        buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    std::string str(buffer);

    (void)message;
    std::cout << GREEN << "[" << str << "]" << RESET << " ";
    std::cout << message << std::endl;
}

#endif
