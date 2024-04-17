#include "command/Command.hpp"
#include "Reply.hpp"

Pong::Pong(Server* srv, bool auth) : Command(srv, auth) {}

Pong::~Pong() {}

void Pong::execute(Client* client, std::vector<std::string> args) {
    if (args.empty()) {
        client->reply(ERR_NOORIGIN(client->getNickname()));
        return;
    }
    client->write("PONG :" + args[0]);
}
