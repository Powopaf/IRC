#include "../../../inc/server/Server.hpp"
#include <iostream>
#include <stdexcept>

void Server::nick(std::vector<std::string> args) {
	if (args.size() != 1)
		throw std::invalid_argument("expect one arg got != 1");
	if (args[0].empty())
		throw std::length_error("empty string");
	_users[uf].setNick(args[0]);
	std::cout << "Set Nick of: " << _users[uf].getUsername() << " to " << args[0] << std::endl;
}