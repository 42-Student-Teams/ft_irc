/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Probook <Probook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:59:42 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/17 14:39:57 by Probook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Parser.hpp"

Parser::Parser(Server* srv) : _srv(srv)
{
    _commands["PASS"] = new Pass(_srv, false);
    _commands["NICK"] = new Nick(_srv, false);
    _commands["USER"] = new User(_srv, false);
    _commands["QUIT"] = new Quit(_srv, false);
   
    
    _commands["NOTICE"] = new Notice(_srv);
    _commands["PRIVMSG"] = new Privmsg(_srv);
    _commands["PART"] = new Part(_srv);
    _commands["LIST"] = new List(_srv);
    _commands["JOIN"] = new Join(_srv);
    _commands["TOPIC"] = new Topic(_srv);
    _commands["WHO"] = new Who(_srv);
    _commands["MODE"] = new Mode(_srv);
    _commands["PING"] = new Ping(_srv);
    _commands["PONG"] = new Pong(_srv);


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

        std::string input = syntax.substr(0, syntax.find(' '));

        try
        {
            std::vector<std::string>    args;
            std::stringstream           line(syntax.substr(input.length(), syntax.length()));
            std::string                 buf;

            Command *cmd = _commands.at(input);

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
            client->reply(ERR_UNKNOWNCOMMAND(client->getNickname(), input));
        }
    }
}
