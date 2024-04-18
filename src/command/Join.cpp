#include "command/Command.hpp"

// syntax: JOIN #<channel>{,<channel>} [<key>{,<key>}]

Join::Join(Server* srv, bool auth) : Command(srv, auth) {}

Join::~Join() {}

void Join::execute(Client* client, std::vector<std::string> args) {
    if (args.empty()) {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "JOIN"));
        return;
    }
    //si y'a plusieurs channels ou keys
    std::vector<std::string> channels = this->split(args[0], ',');
    std::vector<std::string> keys;
    if (args.size() > 1) {
       keys = this->split(args[1], ',');
    }

    for (size_t i = 0; i < channels.size(); ++i) {
        std::string channelName = channels[i];
        std::string key = i < keys.size() ? keys[i] : "";

        // le nom du canal commence par '#'
        if (channelName.empty() || channelName[0] != '#') {
            client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
            continue;
        }

        Channel* channel = this->_srv->getChannel(channelName);
        if (!channel) {
            // Créer canal
            channel = this->_srv->createChannel(channelName, key, client);
        }

        // add user au canal si necessaire
        if (!key.empty() && channel->getKey() != key) {
            client->reply(ERR_BADCHANNELKEY(client->getNickname(), channelName));
            continue;
        }

        channel->addClient(client);
        client->join(channel); // !
    }
}

std::vector<std::string> Join::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(s);
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

