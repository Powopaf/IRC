#include "../../../inc/server/Server.hpp"
#include <cctype>
#include <iostream>
#include <stdexcept>

static std::string extract_cmd(const std::string& msg, size_t& i) {
	std::string cmd;

	if (msg.empty())
		throw std::invalid_argument("Empty message");
	if (msg[0] == ':') {
		size_t prefix_end = msg.find(' ');
		if (prefix_end == std::string::npos || prefix_end == 1)
			throw std::invalid_argument("Invalid message prefix");
		i = prefix_end + 1;
	}
	while (i < msg.size() && msg[i] == ' ')
		i++;
	while (i < msg.size() && std::isupper(static_cast<unsigned char>(msg[i]))) {
		cmd += msg[i];
		i++;
	}
	if (cmd.empty() || (i < msg.size() && msg[i] != ' '))
		throw std::invalid_argument("Invalid command syntax");
	return cmd;
}

static std::vector<std::string> extract_args(const std::string& msg, size_t i) {
	std::vector<std::string> args;
	while (i < msg.size()) {
		while (i < msg.size() && msg[i] == ' ')
			i++; 
		if (i == msg.size())
			break;
		if (msg[i] == ':') {
			args.push_back(msg.substr(i + 1));
			break;
		}
		size_t end = msg.find(' ', i);
		if (end == std::string::npos)
			end = msg.size();
		args.push_back(msg.substr(i, end - i));
		i = end;
	}
	return args;
}

static void exec_msg(const std::string& cmd, std::vector<std::string> args, Server& serv) {
	std::cout<< "cmd is: " << cmd << std::endl;
	
	if(!serv.getUser(serv._getUF()).getLogged()) {
		if (cmd == "JOIN")
			return ; //for now...
		if (cmd == "PASS")
			serv.handlePasswordAuth(args);
		else if (cmd == "NICK")
			serv.handleNick(args);
		else if (cmd == "USER")
			serv.handleUserName(args);
		else if (cmd == "CAP") {
    		if (!args.empty() && args[0] == "LS")
        		serv.sendResponse(":" + serv.gethostname() + " CAP * LS :\r\n");
    		else if (!args.empty() && args[0] == "REQ")
        		serv.sendResponse(":" + serv.gethostname() + " CAP * NAK :" + (args.size() > 1 ? args[1] : "") + "\r\n");
		}
		// else if (cmd == "CAP")
		// 	serv.sendResponse("localhost CAP * LS :\r\n");
		else
			serv.sendResponse(":0.0.0.0 451 * :You have not registered\r\n");
		serv.registerAttempt();
	}
	else
		if (cmd == "PASS" || cmd == "USER")
			serv.sendResponse("You are already registered\r\n");
		else if (cmd == "NICK")
			serv.handleNick(args);
		else if (cmd == "PRIVMSG" || cmd == "MSG")
			serv.privmsg(args);
		else if (cmd == "KICK")
			serv.kick(args);
		else if (cmd == "INVITE")
			serv.invite(args);
		else if (cmd == "TOPIC")
			serv.topic(args);
		else if (cmd == "MODE")
			serv.mode(args);
		else if (cmd == "JOIN")
			serv.join(args);
		// else if (cmd == "PRIVMSG")
		// 	serv.privmsg(args);
		else if (cmd == "PING")
			serv.sendResponse("PONG :" + serv.gethostname() + "\r\n");
		else
			throw std::invalid_argument("Command does not exist");
}

// TODO: add a try catch here

// void Server::handleMessage() {
// 	if (!_users[uf].hasCmd())
// 		return;
// 	std::string msg = _users[uf].getCmd();
// 	size_t i = 0;
// 	if (msg.size() > 512)
// 		throw std::length_error("Message is too long");
// 	std::string cmd = extract_cmd(msg, i);
// 	std::vector<std::string> args = extract_args(msg, i);
// 	std::cout << "Received data from fd " << uf << ": " << msg << std::endl;
// 	exec_msg(cmd, args, *this);
// }

void Server::handleMessage() {
	// Process every complete line currently sitting in this user's buffer.
	// irssi often sends several commands in one packet (e.g. "CAP LS 302\r\nJOIN :\r\n").
	while (_users.count(uf) && _users[uf].hasCmd()) {
		std::string msg = _users[uf].getCmd();

		if (msg.empty())
			continue; // blank line, ignore it

		try {
			size_t i = 0;
			if (msg.size() > 512)
				throw std::length_error("Message is too long");
			std::string cmd = extract_cmd(msg, i);
			std::vector<std::string> args = extract_args(msg, i);
			std::cout << "Received data from fd " << uf << ": " << msg << std::endl;
			exec_msg(cmd, args, *this);
		} catch (const std::exception& e) {
			// One bad line must not stop the remaining lines from being processed
			std::cerr << "[WARN] fd " << uf << ": " << e.what() << std::endl;
		}
	}
}