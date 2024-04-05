#include "command/Command.hpp"

// syntax: TOPIC <channel> [<topic>]

Topic::Topic(Server* srv, bool auth) : Command(srv, auth) {}

Topic::~Topic() {}

void Topic::execute(Client* client, std::vector<std::string> args) {
    if (args.size() < 1) {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
        return;
    }

    std::string channelName = args[0];
    Channel* channel = this->_srv->getChannel(channelName);
    if (!channel) {
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
        return;
    }

    if (args.size() == 1) {
        // Si aucun sujet n'est fourni, afficher le sujet actuel du canal
        std::string topic = channel->getTopic();
        if (topic.empty()) {
            client->reply(RPL_NOTOPIC(client->getNickname(), channelName));
        } else {
            client->reply(RPL_TOPIC(client->getNickname(), channelName, topic));
        }
    } else {
        // Si un sujet est fourni, le définir pour le canal
        std::string topic = args[1];
        channel->setTopic(topic);
        // Diffuser le nouveau sujet à tous les clients du canal
        channel->broadcast(":" + client->getPrefix() + " TOPIC " + channelName + " :" + topic);
    }
}
