#include "../../../inc/server/Server.hpp"
#include <cstddef>
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
	if (!channel.hasKey()) {
		modes += 'k'; 
		params += " " + channel.getKey();
	}
	if (channel.getLimit() > 0) { 
		modes += 'l'; params += " " + (isONch ? std::to_string(channel.getLimit()) : "*"); 
	}
	return modes + params;
}

static void key(bool add, std::string& p, Channel& ch) {
	if (!add) {
		ch.setKey(std::string());
	}
}

static void ops(bool add, std::string& p, Channel& ch) {

}

static void limit(bool add, std::string& p, Channel& ch) {

}

static bool apply(char m, bool add, std::string& p, Channel& ch) {
	switch (m) {
		case 'i':
			ch.setInviteONly(add);
			break;
		case 't':
			ch.setTopicRestricted(add);
			break;
		case 'k':
			key(add, p, ch);
			break;
		case 'o':
			ops(add, p, ch);
			break;
		case 'l':
			limit(add, p, ch);
			break;
		default:
			return false;
	};
	return true;
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
	std::string mode = args[0];
	bool add = true;
	size_t pidx = 1;
	for (size_t i = 0; i < mode.size(); i++) {
		char m = mode[i];
		if (m == '+') {
			add = true;
			continue;
		}
		if (m == '-') {
			add = false;
			continue;
		}
		if ((m == 'o') || (add && (m == 'k' || m == 'l'))) {
			if (pidx >= args.size()) {
				sendResponse(""); // erorr not enought param
				continue;
			}
			pidx++;
		}
		else if (m == 'k' && !add && pidx < args.size())
			pidx++; // "-k oldkey"
		if (!apply(m, add, args[pidx], *_channels[ci])) {
			sendResponse(""); // Unknown mode char
			continue;
		}
	}
}