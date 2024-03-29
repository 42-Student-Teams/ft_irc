#include "command/Command.hpp"

// syntax: PART <channel>{,<channel>} [<Part message>]

class Part : public Command {
public:
    Part(Server* srv, bool auth) : Command(srv, auth) {}
    virtual ~Part() {}

    void execute(Client* client, std::vector<std::string> args) {
        if (args.empty()) {
            client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PART"));
            return;
        }

        std::string channelsStr = args[0];
        std::string partMessage = args.size() > 1 ? args[1] : client->getNickname() + " has left the channel.";

        // Séparer noms de canaux si ils sont listés avec des virgules
        std::stringstream ss(channelsStr);
        std::string channelName;
        while (std::getline(ss, channelName, ',')) {
            Channel* channel = _srv->getChannel(channelName);
            if (channel == nullptr) {
                client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
                continue;
            }

            if (!channel->isClientInChannel(client)) {
                client->reply(ERR_NOTONCHANNEL(client->getNickname(), channelName));
                continue;
            }

            channel->removeClient(client);
            channel->broadcast(":" + client->getPrefix() + " PART " + channelName + " :" + partMessage);
        }
    }
};
