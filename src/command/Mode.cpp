#include "command/Command.hpp"

// MODE <channel> <modes> [limit] [user] [ban mask]
// i : canal sur invitation uniquement.
// t : restrictions sur la commande TOPIC pour les non-opérateurs.
// k : définir ou supprimer la clé du canal (mot de passe).
// o : octroyer ou retirer le statut d'opérateur de canal.
// l : définir ou supprimer la limite d'utilisateurs du canal.

class Mode : public Command {
public:
    Mode(Server* srv, bool auth = true) : Command(srv, auth) {}
    virtual ~Mode() {}

    void execute(Client* client, std::vector<std::string> args){
        if (args.size() < 2) {
            client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
            return;
        }

        std::string channelName = args[0];
        std::string modes = args[1];
        std::string param = args.size() > 2 ? args[2] : "";

        Channel* channel = this->_srv->getChannel(channelName);
        if (!channel) {
            client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
            return;
        }

        channel->setMode(client, modes, param);
    }
};
