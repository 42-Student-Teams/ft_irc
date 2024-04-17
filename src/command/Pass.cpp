#include "command/Command.hpp"

//PASS sert à identifier l'utilisateur au serveur ou une auth supplementaire pour qqch

void Pass::execute(Client* client, std::vector<std::string> args) {
    if (args.empty()) {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PASS"));
        return;
    }

    std::string password = args[0]; // arg 0 = mdp
    if (!this->_srv->checkPassword(password)) {
        client->reply(ERR_PASSWDMISMATCH(client->getNickname()));
        return;
    }

    // Marquer le client comme ayant passé l'authentification par mdp
    if (!client->isPasswordAuthenticated()) {
        client->setPasswordAuthenticated(true);
    } else {
        client->reply("ERR_ALREADYAUTHENTICATED : You are already authenticated.");
    }
}
