#include "../../inc/user/User.hpp"
#include <cstddef>

// bool User::hasCmd() {
// 	return _buffer.find("\r\n") != std::string::npos;
// }

// std::string User::getCmd() {
// 	std::string cmd;
// 	size_t i = 0;
// 	while (_buffer[i] != '\r') {
// 		cmd += _buffer[i];
// 		i++;
// 	}
// 	_buffer.erase(0, i + 2);
// 	return cmd;
// }

bool User::hasCmd() {
	return _buffer.find('\n') != std::string::npos;
}

std::string User::getCmd() {
	size_t end = _buffer.find('\n');
	if (end == std::string::npos)
		return "";

	std::string cmd = _buffer.substr(0, end);
	_buffer.erase(0, end + 1);

	// strip the trailing '\r' if the line ended in "\r\n"
	if (!cmd.empty() && cmd[cmd.size() - 1] == '\r')
		cmd.erase(cmd.size() - 1);
	return cmd;
}