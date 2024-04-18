#include "command/Command.hpp"

// syntax: NOTICE <nickname> :<message>

Notice::Notice(Server* srv, bool auth) : Command(srv, auth) {}

Notice::~Notice() {}

void Notice::execute(Client* client, std::vector<std::string> args) {
    if (args.size() < 2) {
        // pas de mess d'erreur dans IRC donc j'en met pas
        return;
    }

    std::string targetNick = args[0];
    std::string message = args[1];
    Client* targetClient = _srv->getClientStr(targetNick);
    if (targetClient) {
        targetClient->write(":" + client->getPrefix() + " NOTICE " + targetNick + " :" + message);
    }
    // pareil si le client est pas trouvé pas de message
}
