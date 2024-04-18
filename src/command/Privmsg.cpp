#include "command/Command.hpp"

// syntax: PRIVMSG <receiver> :<message>

Privmsg::Privmsg(Server* srv, bool auth) : Command(srv, auth) {}

Privmsg::~Privmsg() {}

void Privmsg::execute(Client* client, std::vector<std::string> args) {
    if (args.size() < 2) {
        client->reply(ERR_NOTENOUGHPARAMS(client->getNickname(), "PRIVMSG"));
        return;
    }

    std::string target = args[0];
    std::string message = args[1];

    // destinataire canal
    Channel* channel = this->_srv->getChannel(target);
    if (channel) {
        if (!channel->isClientInChannel(client)) {
            client->reply(ERR_CANNOTSENDTOCHAN(client->getNickname(), target));
            return;
        }
        channel->broadcast(":" + client->getPrefix() + " PRIVMSG " + target + " :" + message, client);
        return;
    }

    // destinataire utilisateur
    Client* targetClient = this->_srv->getClientStr(target);
    if (targetClient) {
        targetClient->write(":" + client->getPrefix() + " PRIVMSG " + target + " :" + message);
    } else {
        client->reply(ERR_NOSUCHNICK(client->getNickname(), target));
    }
}

