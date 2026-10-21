#include "../../../inc/server/Server.hpp"
#include <string>

static int exist(std::string name, std::vector<Channel*> ch) {
	for (size_t i = 0; i < ch.size(); i++) {
		if (ch[i]->getName() == name)
			return static_cast<int>(i);
	}
	return -1;
}

static std::string modeString(Channel& channel, bool isONch)
{
	std::string modes = "+", params;
	if (channel.isInviteOnly())
		modes += 'i';
	if (channel.getTopicRestrited())
		modes += 't';
	if (!channel.HasPass()) {
		modes += 'k'; 
		params += " " + channel.getPass();
	}
	if (channel.getLimit() > 0) { 
		modes += 'l'; params += " " + (isONch ? std::to_string(channel.getLimit()) : "*"); 
	}
	return modes + params;
}

void Server::mode(std::vector<std::string> args) {
	if (args.size() < 1) {
		//error not enought param
		return;
	}
	int ci = exist(args[0], _channels);
	if (ci == -1) {
		// error channel does not exist
		return;
	}
	if (args.size() == 1) {
		sendResponse(modeString(*_channels[ci], _channels[ci]->hasMember(_users[uf])));
		return;
	}
	if (!_channels[ci]->isOps(_users[uf])) {
		//error not channel operator
		return;
	}
	// parse mode and do the right change
}