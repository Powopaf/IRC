#include "../../inc/Server.hpp"
#include <exception>
#include <iostream>
#include <ostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <poll.h>


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
				uf = _pollFds[i].fd;
				if (uf == _serverFd)
					handleNewConnection();
				else {
					char buffer[512];
					ssize_t n = recv(uf, buffer, sizeof(buffer), 0);
					if (n > 0) {
						_users[uf].setBuf(std::string(buffer, n));
						try {
						handleMessage();
						} catch (const std::exception& e) {
							std::cerr << e.what() << std::endl;
						}
					}
					else if (n <= 0) {
						// close connection
					}
				}
			}
		}
	}
}
