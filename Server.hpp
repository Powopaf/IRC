#pragma once

#include "User.hpp"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>
#include <map>

class User;

class Server{
	private:
		std::map<int, User> dict; // fds, user
		std::string password;
		int port;
	public:
		Server();
		~Server();
		Server(Server const &src);
		Server &operator=(Server const &src);
}