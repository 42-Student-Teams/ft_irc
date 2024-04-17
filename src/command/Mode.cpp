#include "command/Command.hpp"

// MODE <channel> <modes> [limit] [user] [ban mask]
// i : canal sur invitation uniquement.
// t : restrictions sur la commande TOPIC pour les non-opérateurs.
// k : définir ou supprimer la clé du canal (mot de passe).
// o : octroyer ou retirer le statut d'opérateur de canal.
// l : définir ou supprimer la limite d'utilisateurs du canal.

Mode::Mode(Server* srv, bool auth) : Command(srv, auth) {}

Mode::~Mode() {}

void Mode::execute(Client* client, std::vector<std::string> args) {
    if (args.size() < 2) {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
        return;
    }

    std::string channelName = args[0];
    std::string modeChanges = args[1];
    std::string modeParam = args.size() > 2 ? args[2] : "";

    Channel* channel = this->_srv->getChannel(channelName);
    if (!channel) {
        client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
        return;
    }

    if (!channel->isOperator(client)) {
        client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName));
        return;
    }

    // Process each mode change
    bool adding = true;  // Default to adding a mode
    for (size_t i = 0; i < modeChanges.length(); ++i) {
        char mode = modeChanges[i];

        if (mode == '+' || mode == '-') {
            adding = (mode == '+');
            continue;
        }

        switch (mode) {
            case 'i':
                channel->setInviteOnly(adding);
                break;
            case 't':
                channel->setTopicControl(adding);
                break;
            case 'k':
                if (adding && !modeParam.empty()) channel->setKey(modeParam);
                else channel->setKey("");
                break;
            case 'o':
                // Handling of operator privilege
                if (!modeParam.empty()) channel->changeOperatorStatus(client, modeParam, adding);
                break;
            case 'l':
                if (adding && !modeParam.empty()) channel->setLimit(std::stoi(modeParam));
                else channel->setLimit(0);
                break;
            default:
                client->reply(ERR_UNKNOWNMODE(client->getNickname(), std::string(1, mode)));
                return;
        }
    }

    // Confirm mode change to the channel
    channel->broadcastModeChange(client->getPrefix(), modeChanges, modeParam);
}
