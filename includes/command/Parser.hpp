/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:56:21 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/13 13:48:31 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <map>

class Parser;

#include "Command.hpp"

class Parser
{
    private:
        Server*                             _srv;
        std::map<std::string, Command *>    _commands;

        std::string     checkWhitespace(const std::string& str);

    public:
        Parser(Server* srv);
        ~Parser();

        void    excInput(Client* client, const std::string& message);
};

#endif
