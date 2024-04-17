#include "command/Command.hpp"
#include "Reply.hpp"

Ping::Ping(Server* srv, bool auth) : Command(srv, auth) {}

Ping::~Ping() {}

void Ping::execute(Client* client, std::vector<std::string> args) {
    if (args.empty()) {
        client->reply(ERR_NOORIGIN(client->getNickname()));
        return;
    }

    // Répondre avec PONG + paramètre reçu pour maintenir la connexion
    client->write("PONG :" + args[0]);
}
