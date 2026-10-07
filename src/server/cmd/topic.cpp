#include "../../../inc/server/Server.hpp"
#include <stdexcept>

void Server::topic(std::vector<std::string> args) {
	if (args.empty() || args.size() > 2 || args[0].empty()) {
		sendResponse(":" + _hostname + " 461 " + _users[uf].getNick()
			+ " TOPIC :Not enough parameters\r\n");
		return;
	}
	int i = findChannel(args[0]);
	if (i == -1) {
		sendResponse(":" + _hostname + " 403 " + _users[uf].getNick()
			+ " " + args[0] + " :No such channel\r\n");
		return;
	}
	if (args.size() == 1) {
		if (_channels[i]->getTopic().empty())
			sendResponse(":" + _hostname + " 331 " + _users[uf].getNick()
				+ " " + args[0] + " :No topic is set\r\n");
		else
			sendResponse(":" + _hostname + " 332 " + _users[uf].getNick()
				+ " " + args[0] + " :" + _channels[i]->getTopic() + "\r\n");
	}
	else {
		if (_channels[i]->getTopicRestrited() && _users[uf].getAdmin()) {
			_channels[i]->setTopic(args[1]);
		}
		else if (_channels[i]->getTopicRestrited()) {
			sendResponse(":" + _hostname + " 482 " + _users[uf].getNick()
				+ " " + args[0] + " :You're not channel operator\r\n");
			return;
		}
		else {
			_channels[i]->setTopic(args[1]);
		}
		sendResponse(":" + _users[uf].getFullId() + " TOPIC " + args[0]
			+ " :" + args[1] + "\r\n");
	}
}