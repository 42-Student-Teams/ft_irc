/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo <inaranjo@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:59:00 by inaranjo          #+#    #+#             */
/*   Updated: 2024/03/12 15:50:19 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>

class Server
{
    private: 
        int socketFD; // Descripteur de fichier pour le socket d'écoute
        int port; // Port d'écoute

        std::vector<struct pollfd> fds; // Pour la gestion avec poll
        //std::map<int, Client> clients; // Map des clients, clé = descripteur de fichier
       // std::map<std::string, Channel> channels; // Canaux disponibles sur le serveur

        void initServer(); // Initialisation du serveur
        void acceptNewConnection(); // Accepte une nouvelle connexion
        void handleClient(int fd); // Gère les entrées/sorties d'un client
        void closeClient(int fd); // Ferme la connexion avec un client

        Server();
        Server(const Server& cpy);
        Server & operator=(const Server& rhs);

    public:
        Server(const std::string &port, const std::string &password); // construction du serveur par port et mot de pass
        ~Server(); // destrucctor par defaut
        
        /*fonction membre*/
        void run(); //  Lance la boucle principale du serveur
};



#endif
