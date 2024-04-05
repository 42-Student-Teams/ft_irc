#include "command/Command.hpp"

// syntax: WHO [<channel>]

Who::Who(Server* srv, bool auth) : Command(srv, auth) {}

Who::~Who() {}

void Who::execute(Client* client, std::vector<std::string> args) {
    std::vector<Channel*> channels;

    // Si un nom de canal est fourni, lister les utilisateurs de ce canal
    if (!args.empty()) {
        std::string channelName = args[0];
        Channel* channel = this->_srv->getChannel(channelName);
        if (channel) {
            channels.push_back(channel);
        } else {
            // Si le canal n'existe pas, envoyer ERR_NOSUCHCHANNEL
            client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
            return;
        }
    } else {
        // Si aucun canal n'est spécifié, lister les utilisateurs de tous les canaux
        channels = this->_srv->getAllChannels();
    }

    // Parcourir chaque canal et envoyer les informations sur les utilisateurs
    for (std::vector<Channel*>::iterator channelIt = channels.begin(); channelIt != channels.end(); ++channelIt) {
    std::vector<Client*> clients = (*channelIt)->getClients();
    for (std::vector<Client*>::iterator clientIt = clients.begin(); clientIt != clients.end(); ++clientIt) {
        client->reply(RPL_WHOREPLY(client->getNickname(), (*channelIt)->getName(), (*clientIt)->getInfo()));
    }
}

    // Indiquer la fin de la liste WHO
    client->reply(RPL_ENDOFWHO(client->getNickname()));
}
