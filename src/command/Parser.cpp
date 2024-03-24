/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:59:42 by inaranjo          #+#    #+#             */
/*   Updated: 2024/03/24 11:42:06 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Parser.hpp"

Parser::Parser(Server* srv) : _srv(srv)
{
    _commands["NICK"] = new Nick(_srv, false);
    _commands["USER"] = new User(_srv, false);
    _commands["QUIT"] = new Quit(_srv, false);

}

Parser::~Parser ()
{
    for (std::map<std::string, Command*>::iterator it = _commands.begin(), end = _commands.end(); it != end; ++it)
    {
        delete it->second;
    }
}

std::string     Parser::checkWhitespace(const std::string& str)
{
    std::string WHITESPACE = " \n\r\t\f\v";
    std::string result = "";

    size_t  start = str.find_first_not_of(WHITESPACE);
    if (start != std::string::npos)
        result = str.substr(start);

    size_t  end = result.find_last_not_of(WHITESPACE);
    if (end != std::string::npos)
        result = result.substr(0, end + 1);

    return result;
}

void            Parser::processMessage(Client* client, const std::string& message)
{
    std::stringstream   iss(message);
    std::string         syntax;
    
    while (std::getline(iss, syntax))
    {
        syntax = checkWhitespace(syntax);

        std::string name = syntax.substr(0, syntax.find(' '));

        try
        {
            std::vector<std::string>    args;
            std::stringstream           line(syntax.substr(name.length(), syntax.length()));
            std::string                 buf;

            Command *cmd = _commands.at(name);

            while (line >> buf)
                args.push_back(buf);


            if (!client->registrationCheck() && cmd->cmdPermission())
            {
                client->reply(ERR_NOTREGISTERED(client->getNickname()));
                return;
            }
            cmd->execute(client, args);
        }
        catch (const std::exception& e)
        {
            client->reply(ERR_UNKNOWNCOMMAND(client->getNickname(), name));
        }
    }
}
