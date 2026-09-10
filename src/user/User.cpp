#include "../../inc/User.hpp"

bool User::hasCmd() {
	return _buffer.find("\r\n") != std::string::npos;
}