#include "../../../inc/server/Server.hpp"

/*
	remove the operator bool from the user.
	the channel class will keep track of the operator
	if JOIN #new -> create a new channel and put the user admin
	JOIN #1,#2 -> create or join channel 1 and 2
	JOIN #1,#2 key1,key2 -> join channel 1 and 2 with the password key1 for 1 and key2 for 2

	TODO: parse the args again to identifity the channel and the password
	if there is no args that start by 3  = error
*/

void Server::join(std::vector<std::string> args) {

}