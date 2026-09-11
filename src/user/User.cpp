#include "../../inc/user/User.hpp"
#include <cstddef>

bool User::hasCmd() {
	return _buffer.find("\r\n") != std::string::npos;
}

std::string User::getCmd() {
	std::string cmd;
	size_t i = 0;
	while (_buffer[i] != '\r') {
		cmd += _buffer[i];
		i++;
	}
	_buffer.erase(0, i + 2);
	return cmd;
}