#include "../../../inc/server/Server.hpp"
// #include <stdexcept>
// #include <string>
// #include <vector>

static std::string extract_cmd(const std::string& msg, size_t i) {
	std::string cmd;
	size_t j = 0;
	while (std::isupper(msg[i])) {
		cmd += msg[i];
		i++; j++;
	}
	// if (msg[i] != ' ')
	// 	throw std::invalid_argument("The command is invalid: " + msg);
	return cmd;
}

static std::vector<std::string> extract_args(const std::string& msg, size_t i) {
	std::vector<std::string> args;
	while (msg[i]) { //&& msg[i] != '\r') 
		
		std::string a;
		while (msg[i] && msg[i] != ' ') {
			a.push_back(msg[i]);
			i++; 
		}
		args.push_back(a);
		if (msg[i])
			i++;
	}
	return args;
}

static void exec_msg(const std::string& cmd, std::vector<std::string> args, Server& serv) {
	if(!serv.getUsers()[serv._getUF()].getLogged()) {
		if (cmd == "PASS")
			serv.handlePasswordAuth(args);
		else if (cmd == "NICK")
			serv.handleNick(args);
		else if (cmd == "USER")
			serv.handleUserName(args);
		else
			serv.sendResponse("Error: Cant send commands other than PASS, NICK and USER; you have not yet registered\r\n");
		serv.registerAttempt();
	}
	else
		if (cmd == "PASS" || cmd == "USER")
			serv.sendResponse("You are already registered\r\n");
		else if (cmd == "NICK")
			serv.handleNick(args);
		else if (cmd == "PRIVMSG")
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
		else if (cmd == "PRIVMSG")
			serv.privmsg(args);
		else
			throw std::invalid_argument("Command does not exist");
}

// TODO: add a try catch here

void Server::handleMessage() {
	if (!_users[uf].hasCmd())
		return;
	const std::string& msg = _users[uf].getCmd();
	size_t i = 0;
	if (msg.size() > 512)
		throw std::length_error("Message is too long");
	// if (msg[msg.size() - 1] != '\n' && msg[msg.size() - 2] != '\r')
	// 	throw std::invalid_argument("Message does not end with \\r\\n");
	if (msg[0] == ':') {
		while (msg[i] != ' ')
			i++;
	}
	// i++;
	std::string cmd = extract_cmd(msg, i);
	std::cout << "cmd is: " << cmd << std::endl;
	std::vector<std::string> args = extract_args(msg, i);
	std::cout << "Received data from fd " << uf << ": " << msg << std::endl;
	if (cmd.empty())
		throw std::length_error("cmd is empty");
	exec_msg(cmd, args, *this);
}