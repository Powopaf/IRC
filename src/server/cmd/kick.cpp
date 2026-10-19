#include "../../../inc/server/Server.hpp"
#include <cstddef>
#include <vector>
#include <sstream>

static std::vector<std::string> split(std::string input) {
	std::vector<std::string> res;
	std::stringstream stream(input);
	std::string part;
	const char delimiter = ',';
	while (std::getline(stream, part, delimiter)) {
		res.push_back(part);
	}
	return res;
}

static bool checkSender(Channel& channel, int uf) {
	for (size_t i = 0; i < channel.getMembers().size(); i++) {
		if (channel.getMembers()[i]->getFd() == uf)
			return true;
	}
	return false;
}

static bool checkKick(Channel& channel, std::string name) {
	for (size_t i = 0; i < channel.getMembers().size(); i++) {
		if (channel.getMembers()[i]->getNick() == name)
			return true;
	}
	return false;
}

void Server::kick(std::vector<std::string> args) {
	if (args.size() < 2 || args.size() > 3) {
		sendResponse(":" + _hostname + " 461 " + _users[_getUF()].getNick()
					+ " KICK :Not enough parameters\r\n");
		return;
	}
	std::vector<std::string> channels = split(args[0]);
	std::vector<std::string> users = split(args[1]);

	for (size_t i = 0; i < channels.size(); i++) {
		if (i >= users.size())
			continue;
		int c = findChannel(channels[i]);
		if (c == -1) {
			sendResponse(":" + _hostname + " 403 " + _users[_getUF()].getNick()
						+ " " + channels[i] + " :No such channel\r\n");
			continue;
		}
		if (!checkSender(*_channels[c], _getUF())) {
			sendResponse(":" + _hostname + " 442 " + _users[_getUF()].getNick()
						+ " " + channels[i] + " :You're not on that channel\r\n");
			continue;
		}
		if (!_channels[c]->isOps(getUser(_getUF()))) {
			sendResponse(":" + _hostname + " 482 " + getUser(_getUF()).getNick()
						+ " " + _channels[c]->getName() + " :You're not channel operator\r\n");
			continue;
		}
		if (!checkKick(*_channels[c], users[i])) {
			sendResponse(":" + _hostname + " 441 " + getUser(_getUF()).getNick()
						+ " " + users[i] + " " + _channels[c]->getName()
						+ " :They aren't on that channel\r\n");
			continue;
		}
		std::string reason = args.size() == 3 ? args[2] : getUser(_getUF()).getNick();
		std::string message = ":" + getUser(_getUF()).getFullId() + " KICK "
						+ _channels[c]->getName() + " " + users[i]
						+ " :" + reason + "\r\n";
		broadcast(*_channels[c], message);
		User *target = NULL;
		for (size_t member = 0; member < _channels[c]->getMembers().size(); member++)
			if (_channels[c]->getMembers()[member]->getNick() == users[i])
				target = _channels[c]->getMembers()[member];
		_channels[c]->removeUser(*target);
		if (_channels[c]->getMembers().empty()) {
			delete _channels[c];
			_channels.erase(_channels.begin() + c);
		}
	}
}