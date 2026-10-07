#include "../../../inc/server/Server.hpp"
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

/*
	remove the operator bool from the user.
	the channel class will keep track of the operator
	if JOIN #new -> create a new channel and put the user admin
	JOIN #1,#2 -> create or join channel 1 and 2
	JOIN #1,#2 key1,key2 -> join channel 1 and 2 with the password key1 for 1 and key2 for 2

	TODO: parse the args again to identifity the channel and the password
	if there is no args that start by 3  = error
*/

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

static int exist(std::string name, std::vector<Channel*> ch) {
	for (size_t i = 0; i < ch.size(); i++) {
		if (ch[i]->getName() == name)
			return static_cast<int>(i);
	}
	return -1;
}

void Server::join(std::vector<std::string> args) {
	std::vector<std::string> channels_name;
	std::vector<std::string> keys;

	if (args.size() < 1 || args.size() > 2) {
		sendResponse(":" + _hostname + " 461 " + _users[uf].getNick()
			+ " JOIN :Not enough parameters\r\n");
		return;
	}
	channels_name = split(args[0]);
	for (size_t i = 0; i < channels_name.size(); i++) {
		if (channels_name[i].empty() || channels_name[i][0] != '#') {
			sendResponse(":" + _hostname + " 476 " + _users[uf].getNick()
				+ " " + channels_name[i] + " :Bad Channel Mask\r\n");
			return;
		}
	}
	if (args.size() == 2)
		keys = split(args[1]);
	for (size_t i = 0; i < channels_name.size(); i++) {
		int a = exist(channels_name[i], _channels);
		if (a == -1)
			add_Channel(channels_name[i], _users[uf]);
		else if (_channels[a]->HasPass() && i < keys.size())
			add_User_Channel(channels_name[i], keys[i], _users[uf]);
		else if (_channels[a]->HasPass()) {
			sendResponse(":" + _hostname + " 475 " + _users[uf].getNick()
				+ " " + channels_name[i] + " :Cannot join channel (+k)\r\n");
			return;
		}
		else
			add_User_Channel(channels_name[i], _users[uf]);
	}
}