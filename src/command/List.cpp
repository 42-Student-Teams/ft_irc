#include "command/Command.hpp"

// syntax: LIST [<channel>{,<channel>}]

class List : public Command {
public:
    List(Server* srv, bool auth) : Command(srv, auth) {}
    virtual ~List() {}

    void execute(Client* client, std::vector<std::string> args) {
        std::vector<Channel*> channels;

        if (args.empty()) {
            // Si aucun argument, lister tous les canaux
            channels = _srv->getAllChannels();
        } else {
            // else lister seulement les canaux spécifiés
            std::stringstream ss(args[0]);
            std::string channelName;
            while (std::getline(ss, channelName, ',')) {
                Channel* channel = _srv->getChannel(channelName);
                if (channel) {
                    channels.push_back(channel);
                }
            }
        }

        for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
            Channel* channel = *it;
            client->reply(RPL_LIST(client->getNickname(), channel->getName(), channel->getSize(), channel->getTopic()));
        }

        client->reply(RPL_LISTEND(client->getNickname()));
    }
};
