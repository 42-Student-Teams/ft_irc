#include "../includes/Server.hpp"

int	argv_parse(int ac)
{
	if (ac != 3) {
		std::cerr << "Usage: ircserv <port> <password>" << std::endl;
		return (1);
	}
	return (0);
}

int main(int argc, char *argv[])
{
	if (argv_parse(argc))
		return (1);

	int port = std::atoi(argv[1]);
	std::string password(argv[2]);

	Server server(port, password);
	if (!server.setup())
		return (1);

	server.run();

	return 0;
}
