#include "../../inc/Server.hpp"


// Main server loop. Calls poll() on all registered sockets and handles:
//  * - new connection (if poll() returns POLLIN on the server socket)
//  * - client message (if poll() returns POLLIN on a client socket)

void Server::startServ(void) {

	while (1) {
		int poll_count = poll(&_pollFds[0], static_cast<nfds_t>(_pollFds.size()), 6000);
		if (poll_count < 0) {
			perror("poll");
			continue;
		}
		for (std::size_t i = 0; i < _pollFds.size(); ++i) {
			if (_pollFds[i].revents & POLLIN) {
				if (_pollFds[i].fd == _serverFd)
					// handleNewConnection();
				else
					// _handleMessage(i); (using the fd index, but use whatever method you want)
			}
		}
	}
}
