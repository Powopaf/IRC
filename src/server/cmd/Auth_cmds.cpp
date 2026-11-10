#include "../../../inc/server/Server.hpp"
#include <cctype>

void    Server::handlePasswordAuth(std::vector<std::string> args){
    if (args.size() != 1){
        sendResponse("Error: PASS needs one argument only\r\n");
        return;
    }
    if (_password != args[0]){
        sendResponse("Error: Wrong password\r\n");
        return;
    }
    _users[uf].setGotPass();
}

static bool checkNick(const std::string& nick) {
	if (nick.empty() || nick.size() > 9)
		return false;
	const std::string special = "[]\\`_^{}|";
	for (std::size_t i = 0; i < nick.size(); ++i) {
		unsigned char c = static_cast<unsigned char>(nick[i]);
		bool letter = std::isalpha(c) != 0;
		bool nick_special = special.find(nick[i]) != std::string::npos;
		if (i == 0) {
			if (!letter && !nick_special)
				return false;
		} else if (!letter && !std::isdigit(c) && !nick_special && nick[i] != '-') {
			return false;
		}
	}
	return true;
}

void    Server::handleNick(std::vector<std::string> args){
	std::string client = _users[uf].getNick().empty() ? "*" : _users[uf].getNick();
    if (args.empty()) {
        sendResponse(":" + _hostname + " 431 " + client + " :No nickname given\r\n");
        return;
    }
    if (args.size() != 1){
        sendResponse(":" + _hostname + " 461 " + client + " NICK :Not enough parameters\r\n");
        return;
    }
	if (!checkNick(args[0])) {
		sendResponse(":" + _hostname + " 432 " + client + " " + args[0]
			+ " :Erroneous nickname\r\n");
		return;
	}
	for (std::map<int, User>::iterator i = _users.begin(); i != _users.end(); i++) {
		if (i->first != uf && i->second.getNick() == args[0]) {
			sendResponse(":" + _hostname + " 433 " + client + " " + args[0]
				+ " :Nickname is already in use\r\n");
			return;
		}
	}
    _users[uf].setNick(args[0]);
    _users[uf].setGotNick();
}

void Server::handleUserName(std::vector<std::string> args) {
    if (args.size() < 4) {
        sendResponse(":" + _hostname + " 461 * USER :Not enough parameters\r\n");
        return;
    }
    _users[uf].setUsername(args[0]);
    _users[uf].setGotUsername();
}

// void    Server::handleUserName(std::vector<std::string> args){
//     if (args.size() != 1){
//         sendResponse("Error: USER needs one argument only\r\n");
//         return;
//     }
//     _users[uf].setUsername(args[0]);
//     _users[uf].setGotUsername();
// }

// void    Server::registerAttempt(){
//     if (_users[uf].getHasPass() && _users[uf].getHasNick() && _users[uf].getHasUsername()) {
//         _users[uf].setLogged();
//         sendResponse("Congrats you have logged in to this irc server!\r\n");
//     }
// }

void Server::registerAttempt() {
    User& u = _users[uf];
    if (!u.getLogged() && u.getHasPass() && u.getHasNick() && u.getHasUsername()) {
        u.setLogged();
        std::string nick = u.getNick();
        sendResponse(":" + _hostname + " 001 " + nick + " :Welcome to the IRC network, " + u.getFullId() + "\r\n");
        sendResponse(":" + _hostname + " 002 " + nick + " :Your host is " + _hostname + "\r\n");
        sendResponse(":" + _hostname + " 003 " + nick + " :This server was created today\r\n");
        sendResponse(":" + _hostname + " 004 " + nick + " " + _hostname + " 1.0 o itkol\r\n");
    }
}
