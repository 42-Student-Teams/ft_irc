/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:43:06 by inaranjo          #+#    #+#             */
/*   Updated: 2024/03/24 11:59:46 by inaranjo         ###   ########.fr       */
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


#endif
