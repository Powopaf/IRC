#include "../../inc/Server.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <stdexcept>
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
				int fd = _pollFds[i].fd;
				if (fd == _serverFd)
					handleNewConnection();
				else {
					char buffer[512];
					ssize_t n = recv(fd, buffer, sizeof(buffer), 0);
					if (n > 0) {
						_users[fd].setBuf(std::string(buffer, n));
						// check if the user buffer contain "\r\n" and exec the cmd if yes
					}
					else if (n <= 0) {
						// close connection
					}
				}
			}
		}
	}
}

static std::string extract_cmd(std::string msg, size_t i) {
    std::string cmd;
    size_t j = 0;
    while (std::isupper(msg[i])) {
        cmd[j] = msg[i];
        i++; j++;
    }
    if (msg[i] != ' ')
        throw std::invalid_argument("The command is invalid" + msg);
    return cmd;
}

static std::vector<std::string> extract_args(std::string msg, size_t i) {
    std::vector<std::string> args;
    while (msg[i] != '\r') {
        size_t j = 0;
        std::string a;
        while (msg[i] != ' ') {
            a[j] = msg[i];
            i++; j++;
        }
        args.push_back(a);
        i++;
    }
    return args;
}

void Server::handleMessage(const std::string& msg, std::string cmd, std::vector<std::string> args) {
	    size_t i = 0;
    if (msg.size() > 512)
        throw std::length_error("Message is too long");
    if (msg[msg.size() - 1] != '\n' && msg[msg.size() - 2] != '\r')
        throw std::invalid_argument("Message does not end with \\r\\n");
    if (msg[0] == ':') {
        while (msg[i] != ' ')
            i++;
    }
    i++;
    cmd = extract_cmd(msg, i);
    args = extract_args(msg, i);
}