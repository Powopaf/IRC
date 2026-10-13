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

void Server::kick(std::vector<std::string> args) {
	if (args.size() < 2 || args.size() > 3 || args[1][0] == ':')
		sendResponse("");
	std::vector<std::string> channels = split(args[0]);
	std::vector<std::string> users = split(args[1]);

	for (size_t i = 0; i < channels.size(); i++) {
		int c = findChannel(channels[i]);
		if (c == -1)
			// error
		if (!checkSender(*_channels[c], _getUF()))
			//error
		if (/*check target on channel*/)
			//error
		// kick and send to every member of channel
	}
}