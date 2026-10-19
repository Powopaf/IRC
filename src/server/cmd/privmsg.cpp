#include "../../../inc/server/Server.hpp"

void Server::privmsg(std::vector<std::string> args) {
	if (args.size() < 2 || (args[0].empty())){
		sendResponse(":" + _hostname + " 411 " + _users[uf].getNick() + " :No recipient given ( PRIVMSG/MSG )\r\n");
		return;
	}
	User sender = getUser(uf);
	std::vector<std::string> receivers;
	std::string receiver_str = args[0];
	std::size_t pos;

	while ((pos = receiver_str.find(",")) != std::string::npos) {
		receivers.push_back(receiver_str.substr(0, pos));
		receiver_str.erase(0, pos + 1);
	}
	if (!receiver_str.empty())
		receivers.push_back(receiver_str);

	std::string msg;
	for (std::size_t i = 1; i < args.size(); ++i)
		msg += (i == 1 ? "" : " ") + args[i];

	if (msg.empty()){
		sendResponse(":" + _hostname + " 412 " + _users[uf].getNick() + " :No text to send\r\n");
		return; 
	}

	for (std::size_t i = 0; i < receivers.size(); i++){
		std::string& target = receivers[i];
		std::string fullmsg = ":" + _users[uf].getFullId() + " MSG " + target + " :" + msg + "\r\n";

		if (fullmsg.length() > 512){
			sendResponse(":" + _hostname + " NOTICE " + _users[uf].getNick() + " :Input line too long (max 512 bytes)\r\n");
			return;
		}
		if (target[0] == '#') {
			Channel* channel = getChannel(target);
			if (!channel)
				sendResponse(":" + _hostname + " 403 " + _users[uf].getNick() + " " + target + " :No such channel\r\n");
			else {
				if (!channel->isUserOnChannel(sender)){
					sendResponse(":" + _hostname + " 442 " + _users[uf].getNick() + " : " + target + " :You're not on that channel\r\n");
					return;
				}
				sendToChannel(channel, fullmsg, sender);
			}
		}
		else {
			std::map<int, User>::iterator it;
			for (it = _users.begin(); it != _users.end(); it++){
				if (it->second.getNick() == target){
					sendMessage(fullmsg, it->first);
					break;}
				if (it == _users.end())
					sendResponse(":" + _hostname + " 401 " + _users[uf].getNick() + " " + target + " :No such nick/channel\r\n");} 
		}
	}
}