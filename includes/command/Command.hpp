/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Probook <Probook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:43:06 by inaranjo          #+#    #+#             */
/*   Updated: 2024/04/05 16:17:22 by Probook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <numeric>
#include <string>

class Command;

#include "scc/Server.hpp"

/*
classe de base pour différentes commandes du serveur.
Les classes dérivées peuvent implémenter leurs propres comportements de commande 
en redéfinissant la fonction execute().*/ 

/*-----------------ABSTRAITE BASE CLASS------------------*/
class Command
{
    protected:
        Server* _srv;
        bool    _auth;

        Command();
        Command(const Command& src);

    public:
        explicit Command(Server* srv, bool auth = true);
        virtual ~Command();

        bool cmdPermission() const;

        virtual void execute(Client* client, std::vector<std::string> args) = 0;
};

/*------------------------COMMANDES HERITEES DE LA CLASSE ABSTRAITE---------------------*/
class Quit : public Command
{
    public:
        Quit(Server* srv, bool auth);
        ~Quit();

        void    execute(Client* client, std::vector<std::string> args);
};

class User : public Command
{
    public:
        User(Server* srvi, bool auth);
        ~User();

        void    execute(Client* client, std::vector<std::string> args);
};

class Nick : public Command
{
    public:
        Nick(Server* srv, bool auth);
        ~Nick();

        void    execute(Client* client, std::vector<std::string> args);
};

class Notice : public Command
{
    public:
        Notice(Server* srv, bool auth = false); 
        virtual ~Notice();
        void execute(Client* client, std::vector<std::string> args);
};

class Privmsg : public Command
{
    public:
        Privmsg(Server* srv, bool auth = false);
        virtual ~Privmsg();

        void execute(Client* client, std::vector<std::string> args);
};

class Part : public Command 
{
public:
    Part(Server* srv, bool auth = false);
    virtual ~Part();

    void execute(Client* client, std::vector<std::string> args);
};

class List : public Command
{
    public:
        List(Server* srv, bool auth = false);
        virtual ~List();
        void execute(Client* client, std::vector<std::string> args);
};

class Join : public Command {
public:
    Join(Server* srv, bool auth = false);
    virtual ~Join();

    void execute(Client* client, std::vector<std::string> args);

private:
    std::vector<std::string> split(const std::string& s, char delimiter);
};

class Topic : public Command {
public:
    Topic(Server* srv, bool auth = true);
    virtual ~Topic();

    void execute(Client* client, std::vector<std::string> args);
};

class Who : public Command {
public:
    Who(Server* srv, bool auth = false); 
    virtual ~Who();

    void execute(Client* client, std::vector<std::string> args);
};

#endif
