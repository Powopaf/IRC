#include "../../inc/Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password) {
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd == -1)
    	throw std::runtime_error("Error: Failed to create server socket");

	// REUSEADDR -> lets reuse a recently occupied port
	int opt = 1;
    setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // address struct init
    _serverAddress.sin_family = AF_INET; // -> ipv4
    _serverAddress.sin_addr.s_addr = INADDR_ANY; // listen on all interfaces
	uint16_t net_port = htons(_port);
    _serverAddress.sin_port = net_port;

    if (bind(_serverFd, (struct sockaddr *) &_serverAddress, sizeof(_serverAddress)) < 0) {
		close(_serverFd);
		throw std::runtime_error("Error: failed to bind the server socket");
    }

	// get hostname
	socklen_t len = sizeof(_serverAddress);
	if (getsockname(_serverFd, (struct sockaddr*)&_serverAddress, &len) == 0) {
    	_hostname = inet_ntoa(_serverAddress.sin_addr);
	}

    // listening; allowed 5 (doesnt matter which number)
    if (listen(_serverFd, 5) < 0) {
		close(_serverFd);
		throw std::runtime_error("Failed to listen on socket");
	}

	// register server fd
	struct pollfd serv_pollfd;
	serv_pollfd.fd = _serverFd;
	serv_pollfd.events = POLLIN;
	serv_pollfd.revents = 0;
	_pollFds.push_back(serv_pollfd);

	std::cout << "ft_irc server listening on port " << _port << std::endl;
}

void Server::shutdown(void) {

	for (std::size_t i = 0; i < _pollFds.size(); ++i)
		close(_pollFds[i].fd);
	_pollFds.clear();

    close (_serverFd);
}

Server::~Server(void) { close(_serverFd); }