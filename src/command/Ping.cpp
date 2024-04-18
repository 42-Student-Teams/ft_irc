#include "command/Command.hpp"

// syntax: PING :<server>
// command ping for IRC clients to check the connection's liveness and latency

Ping::Ping(Server* srv, bool auth) : Command(srv, auth) {}

Ping::~Ping() {}

void Ping::execute(Client* client, std::vector<std::string> args) {
    if (args.empty()) {
        client->reply(ERR_NOORIGIN(client->getNickname()));
        return;
    }

    // Répondre avec PONG echoing back the argument it received
    client->write("PONG :" + args[0]);
}
