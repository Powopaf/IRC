#include "../../inc/server/Server.hpp"
// #include <arpa/inet.h>
// #include <iostream>
// #include <utility>
// #include <poll.h>

void Server::handleNewConnection(void) {
	struct sockaddr_in user_addr;
	
	socklen_t user_len = sizeof(user_addr);

	int user_fd = accept(_serverFd, (struct sockaddr *)&user_addr, &user_len);

	if (user_fd < 0) {
		perror("accept");
		return;
	}

	struct pollfd user_pollfd;
	user_pollfd.fd = user_fd;
	user_pollfd.events = POLLIN;
	user_pollfd.revents = 0;

	_users.insert(std::make_pair(user_fd, User()));
	_users[user_fd].setFd(user_fd);
	_users[user_fd].setHostname(inet_ntoa(user_addr.sin_addr));
	_pollFds.push_back(user_pollfd);

	std::cout << "NEW user CONNECTED, user fd: " << user_fd << std::endl;
}

void Server::handleDisconnection(void){
	_rmClient();
	close(uf);
	std::cout << "CLIENT DISCONNECTED, client fd: " << uf << std::endl;
}

void Server::_rmClient() {
	for (std::size_t i = 0; i < _pollFds.size(); ++i) {
		if (_pollFds[i].fd == uf) {
			_pollFds.erase(_pollFds.begin() + i);
			break;
			}
		}
	_users.erase(uf);
}

void Server::sendResponse(std::string response) {
	int status = send(uf, response.c_str(), response.length(), 0);
	if (status == -1)
		std::cerr << "send() ERROR: failed to send reponse to socket_fd: " << uf << std::endl;
}

void Server::sendMessage(std::string response, int fd) {
	int status = send(fd, response.c_str(), response.length(), 0);
	if (status == -1)
		std::cerr << "send() ERROR: failed to send reponse to socket_fd: " << uf << std::endl;
}