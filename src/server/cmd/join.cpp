#include "../../../inc/server/Server.hpp"
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

/*
	remove the operator bool from the user.
	the channel class will keep track of the operator
	if JOIN #new -> create a new channel and put the user admin
	JOIN #1,#2 -> create or join channel 1 and 2
	JOIN #1,#2 key1,key2 -> join channel 1 and 2 with the password key1 for 1 and key2 for 2

	TODO: parse the args again to identifity the channel and the password
	if there is no args that start by 3  = error
*/

std::vector<std::string> split(std::string input, bool is_channel) {
	std::vector<std::string> res;
	std::stringstream stream(input);
	std::string part;
	const char delimiter = ',';
	while (std::getline(stream, part, delimiter)) {
		if (is_channel && part[0] != '#')
			throw std::invalid_argument("Channel must have '#' before their name");
		res.push_back(part);
	}
	return res;
}

void Server::join(std::vector<std::string> args) {
	std::vector<std::string> channels_name;
	std::vector<std::string> keys;

	if (args.size() != 1 || args.size() != 2)
		throw std::invalid_argument("Need 1 or 2 args for JOIN to work");

}