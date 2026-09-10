#include "../../../inc/Server.hpp"
#include <stdexcept>
#include <string>
#include <vector>

static std::string extract_cmd(const std::string& msg, size_t i) {
	std::string cmd;
	size_t j = 0;
	while (std::isupper(msg[i])) {
		cmd[j] = msg[i];
		i++; j++;
	}
	if (msg[i] != ' ')
		throw std::invalid_argument("The command is invalid" + msg);
	return cmd;
}

static std::vector<std::string> extract_args(const std::string& msg, size_t i) {
	std::vector<std::string> args;
	while (msg[i] != '\r') {
		size_t j = 0;
		std::string a;
		while (msg[i] != ' ') {
			a[j] = msg[i];
			i++; j++;
		}
		args.push_back(a);
		i++;
	}
	return args;
}

static void exec_msg(const std::string& cmd, std::vector<std::string> args) {

}

void Server::handleMessage() {
	if (!_users[uf].hasCmd())
		return;
	const std::string& msg = _users[uf].getBuf();
	size_t i = 0;
	if (msg.size() > 512)
		throw std::length_error("Message is too long");
	if (msg[msg.size() - 1] != '\n' && msg[msg.size() - 2] != '\r')
		throw std::invalid_argument("Message does not end with \\r\\n");
	if (msg[0] == ':') {
		while (msg[i] != ' ')
			i++;
	}
	i++;
	std::string cmd = extract_cmd(msg, i);
	std::vector<std::string> args = extract_args(msg, i);
}