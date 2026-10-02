#include "../../../inc/server/Server.hpp"
#include <stdexcept>

void Server::topic(std::vector<std::string> args) {
	if (args.size() == 0 || args.size() > 2)
		throw std::invalid_argument("0 or more than 2 args");
	int i = findChannel(args[0]);
	if (i == -1)
		throw std::invalid_argument("channel does not exist");
	if (args.size() == 1) {
		// send message with topic topic = _channels[i].getTopic();
	}
	else {
		if (_channels[i]->getTopicRestrited() && _users[uf].getAdmin()) {
			_channels[i]->setTopic(args[1]);
		}
		else if (_channels[i]->getTopicRestrited())
			throw std::invalid_argument("user does notr have perm");
		else {
			_channels[i]->setTopic(args[1]);
		}
	}
}