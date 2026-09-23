#include "../../inc/channel/Channel.hpp"

Channel::Channel(std::string name) {
	_name = name;
	_topic = "";
	_key = "";
	_inviteOnly = false;
	_topicRestricted = false;
	_usersLimit = 0;
}

Channel::Channel(const Channel& copy) {
	*this = copy;
}

Channel& Channel::operator=(const Channel& other) {
	if (this == &other)
		return *this;
	this->_members = other._members;
	this->_name = other._name;
	return *this;
}

void Channel::addUser(User& user) {
	_members.push_back(&user);
}

void Channel::addOps(User& user) {
	_operators.push_back(&user);
}

bool Channel::HasPass() {
	return !pass.empty();
}