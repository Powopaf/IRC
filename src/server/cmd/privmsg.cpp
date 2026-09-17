#include "../../../inc/server/Server.hpp"

void Server::privmsg(std::vector<std::string> args) {
    if (args.size() < 3 || (args[1].empty())){
        sendResponse("PRIVMSG: wrong arguments");
        return;
    }
    std::vector<std::string> receivers;
	std::string receiver_str = args[1];
	std::size_t pos;

	while ((pos = receiver_str.find(",")) != std::string::npos) {
		receivers.push_back(receiver_str.substr(0, pos));
		receiver_str.erase(0, pos + 1);
    }
	if (!receiver_str.empty())
		receivers.push_back(receiver_str);

	std::string msg;
	for (std::size_t i = 2; i < args.size(); ++i)
		msg += (i == 2 ? "" : " ") + args[i];
    if (msg.empty()){
        sendResponse("PRIVMSG rejected: message is empty\r\n");
        return; 
    }
}