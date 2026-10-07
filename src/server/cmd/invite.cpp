#include "../../../inc/server/Server.hpp"

void Server::invite(std::vector<std::string> args) {
	User& inviter = _users[uf];
	if (args.size() != 2) {
		sendResponse(":" + _hostname + " 461 " + inviter.getNick()
			+ " INVITE :Not enough parameters\r\n");
		return;
	}
	User* invitee = NULL;
	for (std::map<int, User>::iterator user = _users.begin();
		user != _users.end(); user++) {
		if (user->second.getNick() == args[0]) {
			invitee = &user->second;
			break;
		}
	}
	int channel_index = findChannel(args[1]);
	if (invitee == NULL || channel_index == -1) {
		sendResponse(":" + _hostname + " 401 " + inviter.getNick()
			+ " " + args[0] + " :No such nick/channel\r\n");
		return;
	}
	Channel* channel = _channels[channel_index];
	if (!channel->hasMember(inviter)) {
		sendResponse(":" + _hostname + " 442 " + inviter.getNick()
			+ " " + args[1] + " :You're not on that channel\r\n");
		return;
	}
	if (channel->hasMember(*invitee)) {
		sendResponse(":" + _hostname + " 443 " + inviter.getNick()
			+ " " + args[0] + " " + args[1]
			+ " :is already on channel\r\n");
		return;
	}
	if (channel->isInviteOnly() && !channel->hasOperator(inviter)) {
		sendResponse(":" + _hostname + " 482 " + inviter.getNick()
			+ " " + args[1] + " :You're not channel operator\r\n");
		return;
	}
	sendResponse(":" + _hostname + " 341 " + inviter.getNick()
		+ " " + args[0] + " " + args[1] + "\r\n");
	sendMessage(":" + inviter.getFullId() + " INVITE " + args[0]
		+ " " + args[1] + "\r\n", invitee->getFd());
}