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
		if (channel.getMembers()[i]->getUsername() == name)
			return true;
	}
	return false;
}

void Server::kick(std::vector<std::string> args) {
	if (args.size() < 2)
		sendResponse(":" + _hostname + " 461 " + _users[_getUF()].getNick()
					+ " KICK :Not enough parameters\r\n");
	std::vector<std::string> channels = split(args[0]);
	std::vector<std::string> users = split(args[1]);

	for (size_t i = 0; i < channels.size(); i++) {
		int c = findChannel(channels[i]);
		if (c == -1)
			sendResponse(":" + _hostname + " 403 " + _users[_getUF()].getNick()
						+ " " + channels[i] + " :No such channel\r\n");
		if (!checkSender(*_channels[c], _getUF()))
			sendResponse(":" + _hostname + " 442 " + _users[_getUF()].getNick()
						+ " " + channels[i] + " :You're not on that channel\r\n");
		if (!_channels[c]->isOps(getUser(_getUF())))
			sendResponse(":" + _hostname + " 482 " + getUser(_getUF()).getNick()
						+ " " + _channels[c]->getName() + " :You're not channel operator\r\n");
		if (checkKick(*_channels[c], users[i]))
			sendResponse(":" + _hostname + " 441 " + getUser(_getUF()).getNick()
						+ " " + users[i] + " " + _channels[c]->getName()
						+ " :They aren't on that channel\r\n");
		std::string reason = args.size() == 3 ? args[2] : getUser(_getUF()).getUsername();
		std::string message = ":" + getUser(_getUF()).getUsername() + " KICK "
						+ _channels[c]->getName() + " " + users[i]
						+ " :" + reason + "\r\n";
		_channels[c]->broadcast(message);
	}
}