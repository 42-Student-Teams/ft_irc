/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaranjo <inaranjo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 09:58:46 by inaranjo          #+#    #+#             */
/*   Updated: 2024/05/03 13:18:50 by inaranjo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Commands.hpp"

void Commands::handleCommand(int fd, std::string &command)
{
    // if (command.empty() || command == "\n")
    //     return;

    if (_server.getCapStatus() == false && command.find("CAP") != std::string::npos)
    {
        std::string msg = ": CAP * LS :\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        _server.setCapStatus(true);
        // return ;
    }
    if (_server.getCapStatus() == true && command.find("CAP END") != std::string::npos)
        _server.setCapStatus(false);
    if (command.find("CAP ENDPASS") != std::string::npos)
        std::cout << "CAP ENDPASS" << std::endl;

    std::vector<std::string> tokens = _server.parseCmd(command);
    if (tokens.empty())
        return;
    size_t found = command.find_first_not_of(" \t\v");
    if (found != std::string::npos)
        command = command.substr(found);

    // std::string cmdType = tokens[0];

    // print cmdType and command
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (_server.getClient(fd) && _server.getClient(fd)->getRegistered() == false)
        {

            if (tokens[i] == "PASS" || tokens[i] == "pass")
            {
                handlePASS(fd, command);
            }
            else if (tokens[i] == "USER" || tokens[i] == "user")
                handleUSER(fd, command);
            else if (tokens[i] == "NICK" || tokens[i] == "nick")
                handleNICK(fd, command);

            if (_server.getClient(fd)->getPassword() == _server.getPass())
            {
                _server.getClient(fd)->setLogedin(true);
                _server.sendMsg(RPL_PASSACCEPTED(_server.getClient(fd)->getNickName()), fd);
            }
            if (_server.getClient(fd)->getNickName() != "" && _server.getClient(fd)->getUserName() != "")
                _server.getClient(fd)->setReceivedInfo(true);

            if (_server.getClient(fd)->getLogedIn() == true && _server.getClient(fd)->getNickReceived() == true && _server.getClient(fd)->BigWelcomeSent() == false)
            {
                _server.getClient(fd)->setRegistered(true);
                _server.sendMsg(RPL_CONNECTED(_server.getClient(fd)->getNickName()), fd);
                _server.getClient(fd)->setBigWelcomeSent(true);
            }
        }
        else
        {
            if (tokens[i] == "JOIN" || tokens[i] == "join")
                handleJOIN(fd, command);
            else if (tokens[i] == "PART" || tokens[i] == "part")
                handlePART(fd, command);
            else if (tokens[i] == "PRIVMSG" || tokens[i] == "privmsg")
                handlePRIVMSG(fd, command);
            else if (tokens[i] == "TOPIC" || tokens[i] == "topic")
                handleTOPIC(fd, command);
            else if (tokens[i] == "MODE" || tokens[i] == "mode")
                handleMODE(fd, command);
            else if (tokens[i] == "PING" || tokens[i] == "ping")
                handlePING(fd, command);
            else if (tokens[i] == "WHO" || tokens[i] == "who")
                handleWHO(fd, command);
            else if (tokens[i] == "KICK" || tokens[i] == "kick")
                handleKICK(fd, command);
            else if (tokens[i] == "NOTICE" || tokens[i] == "notice")
                handleNOTICE(fd, command);
            else if (tokens[i] == "INVITE" || tokens[i] == "invite")
                handleINVITE(fd, command);
            else if (tokens[i] == "QUIT" || tokens[i] == "quit")
                handleQUIT(fd, command);
            // else
            //     _server.sendMsg(ERR_CMDNOTFOUND(_server.getClient(fd)->getNickName(), tokens[i]), fd);
        }
    }
}

void Commands::handlePASS(int fd, std::string &cmd)
{
    std::vector<std::string> tokens = _server.parseCmd(cmd);
    Client *cli = _server.getClient(fd);

    // Vérifier si la commande a suffisamment de paramètres et traiter le mot de passe.

    if (tokens[1].empty())
    {
        _server.sendMsg(ERR_NOTENOUGHPARAM2("*"), fd);
    }
    else if (cli->getRegistered())
    {
        _server.sendMsg(ERR_ALREADYREGISTERED(cli->getNickName()), fd);
    }
    else
    {
        _server.getClient(fd)->setPassword(tokens[1]); // Mot de passe positionné à l'index 1
    }
}

void Commands::handleNICK(int fd, std::string &command)
{
    std::vector<std::string> tokens = _server.parseCmd(command);
    if (tokens.size() < 2)
    {
        _server.sendMsg(ERR_NONICKNAME2("*"), fd);
        return;
    }

    std::string newNick = tokens[1];
    Client *cli = _server.getClient(fd);

    // Vérifie si le pseudonyme est valide
    if (!checkNickname(newNick))
    {
        _server.sendMsg(ERR_ERRONEUSNICK(newNick), fd);
        return;
    }

    // Vérifie les collisions de pseudonyme
    if (_server.isNicknameInUse(newNick))
    {
        if (cli->getNickName() != newNick) // Assurez-vous que le pseudonyme est différent de l'actuel
        {
            std::cout << "Nickname already in use: " << newNick << std::endl;
            std::cout << "Client <" << fd << "> disconnected" << std::endl;
            _server.sendMsg(ERR_NICKNAMEINUSE(newNick), fd);
            return;
        }
    }
    else
    {
        if (cli->getRegistered()) // Ajoute une condition pour vérifier si le client est enregistré
        {

            std::string oldNick = cli->getNickName();
            cli->setNickName(newNick); // Met à jour le pseudonyme
            // cli->setReceivedInfo(true); // Marque le pseudonyme comme reçu
            if (!oldNick.empty() && oldNick != newNick)
            {
                _server.sendMsg(RPL_NICKCHANGE(oldNick, newNick), fd); // Informe le client du changement
            }
        }
        cli->setNickName(newNick); // Met à jour le pseudonyme
    }
}

bool Commands::checkNickname(std::string &nickname)
{
    if (nickname.empty() || nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':')
        return false;
    for (size_t i = 1; i < nickname.length(); i++)
    {
        if (!std::isalnum(nickname[i]) && nickname[i] != '_')
            return false;
    }
    return true;
}

bool Commands::areDuplicateNames(std::string &nickname)
{
    return _server.isNicknameInUse(nickname);
}

void Commands::handleUSER(int fd, std::string &command)
{
    std::vector<std::string> tokens = _server.parseCmd(command);
    Client *user = _server.getClient(fd);

    if (user && tokens.size() < 5)
    {
        _server.sendMsg(ERR_NOTENOUGHPARAM(user->getNickName()), fd);
        return;
    }
    else if (user && !user->getUserName().empty())
    {
        _server.sendMsg(ERR_ALREADYREGISTERED(user->getNickName()), fd);
        return;
    }
    else
    {
        user->setUserName(tokens[1]);
    }
}

void Commands::handleQUIT(int fd, std::string &command)
{
    // Définir le message de déconnexion par défaut
    std::string quitMessage = "no reason provided";

    // Essayer de trouver un message après "QUIT "
    size_t pos = command.find("QUIT ");
    if (pos != std::string::npos && pos + 5 < command.size())
    {
        quitMessage = command.substr(pos + 5);
        // Supprimer tout espace initial inutile qui pourrait affecter le message
        quitMessage.erase(0, quitMessage.find_first_not_of(" "));
    }

    // vérification de l'existence du client
    Client *client = _server.getClient(fd);
    if (!client)
    {
        return; // Si le client n'existe pas, arrêtez la méthode ici
    }

    // Construction du message d'information du client
    std::string clientInfo = ":" + client->getNickName() + "!~" + client->getUserName() + "@localhost";
    std::string message = clientInfo + " QUIT :" + quitMessage + "\r\n";

    // Envoi du message QUIT à tous les canaux auxquels le client est connecté
    std::vector<Channel> &channels = _server.getChannels();

    // std::string clientNickName = client->getNickName();
    for (size_t i = 0; i < channels.size(); ++i)
    {
        if (channels[i].isClientInChannel(client->getFD()))
        {
            channels[i].sendMsgToAll(message, fd); // Envoyer le message de QUIT à tous sauf au client qui quitte
            channels[i].rmClientFd(fd);            // Supprimer le client du canal
            if (channels[i].getNbClients() == 0)
            {
                channels.erase(channels.begin() + i--); // Supprimer le canal s'il est vide
            }
        }
    }
    std::cout << RED << "Client <" << fd << "> disconnected: " << RESET << quitMessage << std::endl;
    _server.rmClient(fd);
    _server.rmPfds(fd);
    close(fd);
}

void Commands::handlePART(int fd, std::string &command)
{
    std::vector<std::string> tokens = _server.parseCmd(command);
    Client *client = _server.getClient(fd);

    if (tokens.size() < 2)
    {
        _server.sendMsg(ERR_NEEDMOREPARAMS("*", "PART"), fd);
        return;
    }

    // print tokens
    std::string channelName = tokens[1]; // Utiliser directement le nom du canal sans boucle
    std::string partMessage = tokens.size() > 2 ? command.substr(command.find(tokens[2])) : client->getNickName() + " has left the channel.\r\n";

    // Check if the channel name starts with '#'
    if (channelName.empty() || channelName[0] != '#')
    {
        _server.sendMsg("ERROR " + client->getNickName() + " :Missing # to leave a channel\r\n", fd);
        return; // Simplement retourner si le format n'est pas correct
    }

    Channel *channel = _server.getChannel(channelName);

    if (channel)
    {
        std::string nickName = client->getNickName();
        // TODO : when quit twice it sends message to all clients but not in channel
        if (!channel->isClientInChannel(client->getFD()))
        {
            _server.sendMsg(ERR_NOTONCHANNEL(nickName, channelName + "\r\n"), fd);
            return;
        }

        if (channel->isOperator(nickName))
            channel->removeOperator(client);
        else
            channel->removeClient(client->getFD());
        channel->sendMsgToAll(":" + client->getHostname() + " PART " + channelName + " :" + partMessage + "\r\n");
        // Supprimer le canal s'il est vide
        if (channel->getNbClients() == 0)
            _server.rmChannel(channelName);
    }
    else
        _server.sendMsg(ERR_NOSUCHCHANNEL(client->getNickName(), channelName + "\r\n"), fd);
}

std::vector<std::string> Commands::split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::istringstream tokenStream(s);
    std::string token;
    while (std::getline(tokenStream, token, delimiter))
    {
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

void Commands::handleJOIN(int fd, std::string &command)
{

    std::vector<std::string> tokens = _server.parseCmd(command);
    Client *client = _server.getClient(fd);
    if (tokens.size() < 2)
    {
        _server.sendMsg(ERR_NEEDMOREPARAMS(client->getNickName(), "JOIN"), fd);
        return;
    }

    std::vector<std::string> channels = split(tokens[1], ',');
    std::vector<std::string> keys;
    if (tokens.size() > 2)
    {
        keys = split(tokens[2], ',');
    }

    for (size_t i = 0; i < channels.size(); ++i)
    {
        std::string channelName = channels[i];
        std::string key = i < keys.size() ? keys[i] : "";

        Channel *channel = _server.getChannel(channelName);
        if (channel)
        {
            if (channel->isClientInChannel(fd))
            {
                _server.sendMsg(ERR_ALREADYONCHANNEL(client->getNickName(), channelName), fd);
                return;
            }
            // Vérifie si le canal est en mode invitation seulement
            if (channel->getInviteOnly())
            {
                if (!channel->isClientInvited(fd))
                {
                    _server.sendMsg(ERR_INVITEONLYCHAN(client->getNickName(), channelName), fd);
                    return; // Ne pas permettre au client de rejoindre le canal
                }
            }
            // Vérifie si le canal est en mode clé
            if (channel->getKey() != 0 && key != std::to_string(channel->getKey()))
            {
                _server.sendMsg(ERR_BADCHANNELKEY(client->getNickName(), channelName), fd);
                return; // Clé incorrecte, ne pas permettre au client de rejoindre le canal
            }
            // Vérifie si le canal est plein
            if (channel->getMaxClients() != 0 && channel->getNbClients() >= channel->getMaxClients())
            {
                _server.sendMsg(ERR_CHANNELISFULL(client->getNickName(), channelName), fd);
                return; // Le canal est plein, ne pas permettre au client de rejoindre le canal
            }

            channel->addClient(client);
            channel->sendMsgToAll(client->getNickName() + " has join the channel : " + channelName + "\r\n");
            // channel->sendMsgToAll(client->getNickName() + " " + "has join the channel :" + channelName, fd);
        }
        else if (channelName[0] == '#')
        {
            channel = _server.createChannel(channelName, key, client);
            channel->addOperator(client);
            channel->sendMsgToAll(client->getNickName() + " has join the channel : " + channelName + "\r\n");
        }
        else
            _server.sendMsg(ERR_NOSUCHCHANNEL(client->getNickName(), channelName), fd);

        if (!key.empty() && channel->getKey() != std::stoi(key))
        {
            _server.sendMsg(ERR_BADCHANNELKEY(client->getNickName(), channelName), fd);
            continue;
        }
    }
}

void Commands::handleNOTICE(int fd, std::string &command)
{
    std::vector<std::string> tokens = _server.parseCmd(command);
    if (tokens.size() < 3)
    {
        return; // No error message is sent for NOTICE
    }

    std::string targetNick = tokens[1];
    std::string message = command.substr(command.find(tokens[2])); // Capture the entire message after the targetNick

    Client *targetClient = _server.getNickClient(targetNick);
    if (targetClient)
    {
        targetClient->write(":" + _server.getClient(fd)->getHostname() + " NOTICE " + targetNick + " :" + message);
    }
}

void Commands::handlePRIVMSG(int fd, std::string &command)
{
    std::vector<std::string> tokens = _server.parseCmd(command);
    if (tokens.size() < 3)
    {
        _server.sendMsg(ERR_NOTENOUGHPARAMS(_server.getClient(fd)->getNickName(), "PRIVMSG"), fd);
        return;
    }
    std::string target = tokens[1];
    std::string message = command.substr(command.find(tokens[2]));

    Channel *channel = _server.getChannel(target);
    if (channel)
    {
        // std::string clientNick = _server.getClient(fd)->getNickName();
        if (!channel->isClientInChannel(fd))
        {
            _server.sendMsg(ERR_CANNOTSENDTOCHAN(_server.getClient(fd)->getNickName(), target), fd);
            return;
        }
        channel->sendMsgToAll(":" + _server.getClient(fd)->getHostname() + " PRIVMSG " + target + " :" + message + "\r\n", fd);
        return;
    }
    Client *targetClient = _server.getNickClient(target);
    if (targetClient)
    {
        targetClient->write(":" + _server.getClient(fd)->getHostname() + " PRIVMSG " + target + " :" + message + "\r\n");
    }
    else
    {
        _server.sendMsg(ERR_NOSUCHNICK(_server.getClient(fd)->getNickName(), target), fd);
    }
}

void Commands::handleTOPIC(int fd, std::string &command)
{
    std::vector<std::string> tokens = _server.parseCmd(command);
    Client *client = _server.getClient(fd);
    if (tokens.size() < 2)
    {
        _server.sendMsg(ERR_NEEDMOREPARAMS(client->getNickName(), "TOPIC"), fd);
        return;
    }

    std::string channelName = tokens[1];
    Channel *channel = _server.getChannel(channelName);
    // std::cout << "channelName: " << channelName << std::endl;
    if (!channel)
    {
        _server.sendMsg(ERR_NOSUCHCHANNEL(client->getNickName(), channelName), fd);
        return;
    }

    // std::cout << "tokens.size(): " << tokens.size() << std::endl;
    if (tokens.size() == 3)
    {
        std::string topic = channel->getTopicName();
        std::cout << "topic: " << topic << std::endl;
        if (topic.empty())
            _server.sendMsg(RPL_NOTOPIC(client->getNickName(), channelName), fd);
        else
            _server.sendMsg(RPL_TOPIC(client->getNickName(), channelName, topic), fd);
    }
    else if (tokens.size() >= 3) // Vérifier si au moins 3 tokens sont présents
    {
        // Vérifier si le mode "t" est activé
        if (channel->getTopicRestric())
        {
            // Vérifier si le client est autorisé à modifier le sujet
            if (!channel->isOperator(client->getNickName()))
            {
                _server.sendMsg(ERR_CHANOPRIVSNEEDED(client->getNickName(), channelName), fd);
                return;
            }
        }
        std::string topic = command.substr(command.find(tokens[2]));
    
        channel->setTopicName(topic);
        channel->sendMsgToAll(":" + client->getHostname() + " TOPIC " + channelName + " :" + topic + "\r\n");
    }
    else // Gérer le cas où 'tokens' ne contient pas suffisamment d'éléments
        _server.sendMsg(ERR_NEEDMOREPARAMS(client->getNickName(), "TOPIC"), fd);
}

// Function to trim whitespace from the start and end of a string
static inline std::string trim(const std::string &str)
{
    const char *whitespace = " \t\n\r\f\v"; // Define whitespace characters
    std::string::size_type start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) // If the string is all whitespace
        return "";                  // Return an empty string

    std::string::size_type end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

void Commands::handleKICK(int fd, std::string &command)
{
    std::vector<std::string> tokens = _server.parseCmd(command);
    if (tokens.size() < 3)
    {
        _server.sendMsg(ERR_NOTENOUGHPARAMS(_server.getClient(fd)->getNickName(), "KICK"), fd);
        return;
    }
    std::string channelName = tokens[1];
    std::string targetNickName = tokens[2];
    std::string msg = command.substr(command.find(targetNickName) + targetNickName.length());
    msg = trim(msg); // Assuming you've implemented trim as suggested earlier.

    // Rechercher le canal
    Channel *channel = _server.getChannel(channelName);
    // Vérifier si le canal existe
    if (channel == nullptr)
    {
        _server.sendMsg(ERR_NOSUCHCHANNEL(_server.getClient(fd)->getNickName(), channelName), fd);
        return;
    }
    // Vérifier si le client est dans le canal
    if (!channel->isClientInChannel(fd))
    {
        _server.sendMsg(ERR_NOTONCHANNEL(_server.getClient(fd)->getNickName(), channelName), fd);
        return;
    }
    // Vérifier si le client est admin
    if (!channel->isOperator(_server.getClient(fd)->getNickName()))
    {
        _server.sendMsg(ERR_CHANOPRIVSNEEDED(_server.getClient(fd)->getNickName(), channelName), fd);
        return;
    }
    // Vérifier si le client à expulser est dans le canal
    Client *targetClient = channel->getClientInChannel(targetNickName);
    if (targetClient == nullptr)
    {
        _server.sendMsg(ERR_USERNOTINCHANNEL(_server.getClient(fd)->getNickName(), targetNickName, channelName), fd);
        return;
    }
    int targetFd = targetClient->getFD();
    // Envoyer le message à tous les clients du canal
    channel->sendMsgToAll(":" + _server.getClient(fd)->getNickName() + " KICK " + channelName + " " + targetNickName + " :" + msg + "\r\n");
    // Expulser le client
    channel->rmClientFd(targetFd);
    channel->removeClient(targetFd);
}

void Commands::handleWHO(int fd, std::string &command)
{
    std::vector<std::string> tokens = _server.parseCmd(command);

    // Vérifier si le nom du canal est fourni
    std::string channelName;
    if (tokens.size() > 1)
        channelName = tokens[1]; // Le deuxième élément devrait être le nom du canal
    else
    {
        _server.sendMsg("Usage: NAMES <channel_name>", fd);
        return;
    }

    // Obtenir l'instance du canal
    Channel *channel = _server.getChannel(channelName);
    if (!channel)
    {
        _server.sendMsg("No such channel: " + channelName, fd);
        return;
    }

    // Obtenir la liste des utilisateurs dans le canal
    std::string userList = channel->getChannelList(); // Utilise la méthode de Channel pour obtenir la liste des utilisateurs

    // Préparer et envoyer la réponse
    std::string response = "Users in " + channelName + ": " + userList;
    _server.sendMsg(response, fd);
}

void Commands::handleMODE(int fd, std::string &command)
{
    std::vector<std::string> tokens = _server.parseCmd(command);
    Client *client = _server.getClient(fd);
    if (tokens.size() < 3)
    {
        _server.sendMsg(ERR_NEEDMOREPARAMS(client->getNickName(), "MODE"), fd);
        return;
    }

    std::string channelName = tokens[1];
    std::string modeChanges = tokens[2];
    std::string modeParam = tokens.size() > 3 ? tokens[3] : "";
    // print tokens
    for (size_t i = 0; i < tokens.size(); i++)
    {
        std::cout << "tokens[" << i << "]: " << tokens[i] << std::endl;
    }

    Channel *channel = _server.getChannel(tokens[1]);
    if (!channel)
    {
        _server.sendMsg(ERR_NOSUCHCHANNEL(client->getNickName(), channelName), fd);
        return;
    }

    if (!channel->isOperator(client->getNickName()))
    {
        _server.sendMsg(ERR_CHANOPRIVSNEEDED(client->getNickName(), channelName), fd);
        return;
    }

    // Process each mode change
    bool adding = true;
    for (size_t i = 0; i < modeChanges.length(); ++i)
    {
        char mode = modeChanges[i];

        if (mode == '+' || mode == '-')
        {
            adding = (mode == '+');
            continue;
        }
        // Apply the mode change
        switch (mode)
        {
        case 'i':
            channel->setInviteOnly(adding);
            break;
        case 't':
            channel->setTopicControl(adding);
            break;
        case 'k':
            if (adding && !modeParam.empty())
            {
                int keyValue = std::stoi(modeParam); // Convert string to integer
                channel->setKey(keyValue);
            }
            else
            {
                channel->setKey(0); // Clear the key
            }
            break;
        case 'o':
            if (!modeParam.empty())
                channel->changeOperatorStatus(client, modeParam, adding);
            break;
        case 'l':
            if (adding && !modeParam.empty())
                channel->setMaxUsers(std::stoi(modeParam));
            else
                channel->setMaxUsers(0);
            break;
        default:
            _server.sendMsg(ERR_UNKNOWNMODE(client->getNickName(), tokens[1], std::string(1, mode)), fd);
            return;
        }
    }

    channel->broadcastModeChange(client->getHostname(), modeChanges, modeParam);
}

void Commands::handleINVITE(int fd, std::string &command)
{
    std::vector<std::string> tokens = _server.parseCmd(command);
    if (tokens.size() < 3)
        return;

    std::string inviterNick = _server.getClient(fd)->getNickName();
    std::string targetNick = tokens[1];
    std::string channelName = tokens[2];

    Channel *channel = _server.getChannel(channelName);
    if (!channel)
    {
        // Le canal n'existe pas
        _server.sendMsg(ERR_NOSUCHCHANNEL(inviterNick, channelName), fd);
        return;
    }

    if (!channel->getClientInChannel(inviterNick))
    {
        // Le client n'est pas dans le canal
        _server.sendMsg(ERR_NOTONCHANNEL(inviterNick, channelName), fd);
        return;
    }
    if (channel->getClientInChannel(targetNick))
    {
        // Le client cible est déjà dans le canal
        _server.sendMsg(ERR_ALREADYONCHANNEL(targetNick, channelName), fd);
        return;
    }

    // Vérifier si le client cible existe
    Client *targetClient = _server.getNickClient(targetNick);
    if (!targetClient)
    {
        // Le client cible n'existe pas
        _server.sendMsg(ERR_NOSUCHNICK(inviterNick, targetNick), fd);
        return;
    }
    if (channel->getInviteOnly() && !channel->isOperator(inviterNick))
    {
        // Le canal est en mode invitation seulement et l'invitant n'est pas un opérateur
        _server.sendMsg(ERR_NOTOPERATOR(channelName), fd);
        return;
    }

    // Envoyer l'invitation au client cible
    channel->addToInvitedList(targetClient);
    // Envoyer un message au client invité pour l'informer de l'invitation
    _server.sendMsg(RPL_INVITATIONSENT(inviterNick, targetNick, channelName), fd);
    targetClient->write(RPL_INVITING(inviterNick, channelName));
}

void Commands::handlePING(int fd, std::string &command)
{
    std::vector<std::string> tokens = _server.parseCmd(command);
    if (tokens.size() < 2)
    {
        _server.sendMsg(ERR_NOORIGIN(_server.getClient(fd)->getNickName()), fd);
        return;
    }

    std::string pingArgument = tokens[1];
    _server.sendMsg("PONG :" + pingArgument, fd);
}