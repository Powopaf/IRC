#include "../../inc/server/Server.hpp"

void Server::add_Channel(std::string name, User ops) {
	_channels.push_back(new Channel(name));
	_channels.back()->addOps(ops);
}

void Server::add_User_Channel(std::string name, User user) {
	for (size_t i = 0; i < _channels.size(); i++) {
		if (name == _channels[i]->getName()) {
			_channels[i]->addUser(user);
		}
	}
}

void Server::add_User_Channel(std::string name, std::string pass, User user) {
	for (size_t i = 0; i < _channels.size(); i++) {
		if (name == _channels[i]->getName() &&
			_channels[i]->getPass() == pass)
			_channels[i]->addUser(user);
	}
}