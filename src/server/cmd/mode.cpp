#include "../../../inc/server/Server.hpp"
#include <cerrno>
#include <climits>
#include <cstddef>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <unistd.h>

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
	ch.setKey(p);
}

static int ops(bool add, std::string& p, Channel& ch) {
	if (!ch.hasMember(p)) {
		return 2;
	}
	add ? ch.addOps(p) : ch.rmOps(p);
	return 0;
}

static unsigned long toUI(const std::string& v) {
	char *end;
	if (v.empty())
		throw std::invalid_argument("");
	errno = 0;
	unsigned long number = std::strtoul(v.c_str(), &end, 10);
	if (*end != '\0')
		throw std::invalid_argument("");
	if (errno == ERANGE || number > UINT_MAX)
		throw std::out_of_range("");
	return static_cast<unsigned int>(number);
}

static int limit(bool add, std::string& p, Channel& ch) {
	try {
	add ? ch.setLimit(toUI(p)) : ch.setLimit(0);
	} catch (const std::string e) {
		(void)e;
		return 3;
	}
	return 0;
}

static int apply(char m, bool add, std::string& p, Channel& ch) {
	int err = 0;
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
			err = ops(add, p, ch);
			break;
		case 'l':
			err = limit(add, p, ch);
			break;
		default:
			err = 1;
			break;
	};
	return err;
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
		int err = apply(m, add, args[pidx], *_channels[ci]);
		switch (err) {
			case 1:
				sendResponse(""); //error unknown mode char
				break;
			case 2:
				sendResponse(""); //error target nick not on channel
				break;
			case 3:
				sendResponse(""); // error 
				break;
			case 0:
				sendResponse(""); // success
				break;
		}
	}
}