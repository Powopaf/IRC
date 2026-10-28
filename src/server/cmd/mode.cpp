#include "../../../inc/server/Server.hpp"
#include <cerrno>
#include <climits>
#include <cstddef>
#include <cstdlib>
#include <sstream>
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

static std::string toString(unsigned int value) {
    std::ostringstream stream;
    stream << value;
    return stream.str();
}

static std::string modeString(Channel& channel, bool isONch)
{
	std::string modes = "+", params;
	if (channel.isInviteOnly())
		modes += 'i';
	if (channel.getTopicRestrited())
		modes += 't';
	if (channel.hasKey()) {
		modes += 'k'; 
		params += " *";
	}
	if (channel.getLimit() > 0) { 
		modes += 'l'; params += " " + (isONch ? toString(channel.getLimit()) : "*"); 
	}
	return modes + params;
}

static void key(bool add, std::string& p, Channel& ch) {
	ch.setKey(add ? p : std::string());
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
	} catch (const std::exception& e) {
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
		sendResponse(":" + _hostname + " 461 " + _users[uf].getNick()
			+ " MODE :Not enough parameters\r\n");
		return;
	}
	int ci = exist(args[0], _channels);
	if (ci == -1) {
		sendResponse(":" + _hostname + " 403 " + _users[uf].getNick()
			+ " " + args[0] + " :No such channel\r\n");
		return;
	}
	if (args.size() == 1) {
		sendResponse(modeString(*_channels[ci], _channels[ci]->hasMember(_users[uf])));
		return;
	}
	if (!_channels[ci]->hasMember(_users[uf])) {
		sendResponse(":" + _hostname + " 442 " + _users[uf].getNick()
			+ " " + args[0] + " :You're not on that channel\r\n");
		return;
	}
	if (!_channels[ci]->isOps(_users[uf])) {
		sendResponse(":" + _hostname + " 482 " + _users[uf].getNick()
			+ " " + args[0] + " :You're not channel operator\r\n");
		return;
	}
	std::string mode = args[1];
	bool add = true;
	size_t pidx = 2;
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
		std::string parameter;
		if (m == 'o' || (add && (m == 'k' || m == 'l')) || (!add && m == 'k')) {
			if (pidx >= args.size()) {
				sendResponse(":" + _hostname + " 461 " + _users[uf].getNick()
					+ " MODE :Not enough parameters\r\n");
				return;
			}
			parameter = args[pidx++];
		}
		int err = apply(m, add, parameter, *_channels[ci]);
		switch (err) {
			case 1:
				sendResponse(":" + _hostname + " 472 " + _users[uf].getNick()
					+ " " + std::string(1, m) + " :is unknown mode char to me\r\n");
				break;
			case 2:
				sendResponse(":" + _hostname + " 441 " + _users[uf].getNick()
					+ " " + parameter + " " + args[0] + " :They aren't on that channel\r\n");
				break;
			case 3:
				sendResponse(":" + _hostname + " 696 " + _users[uf].getNick()
					+ " " + args[0] + " " + std::string(1, m)
					+ " :Invalid mode parameter\r\n");
				break;
			case 0:
				{
					std::string message = ":" + _users[uf].getFullId()
						+ " MODE " + args[0] + " " + (add ? "+" : "-")
						+ std::string(1, m);
					if (!parameter.empty() && (m == 'k' || m == 'l'))
						message += " " + parameter;
					message += "\r\n";
					broadcast(*_channels[ci], message);
				}
				break;
		}
	}
}