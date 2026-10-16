#include "../../inc/channel/Channel.hpp"
#include <iostream>

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

bool Channel::hasMember(const User& user) const {
	for (size_t i = 0; i < _members.size(); i++)
		if (_members[i]->getFd() == user.getFd()){
			std::cout << "meowmeow" << std::endl;
			return true;
		}
	return false;
}

bool Channel::hasOperator(const User& user) const {
	for (size_t i = 0; i < _operators.size(); i++)
		if (_operators[i]->getFd() == user.getFd()){
			std::cout << "meowmeow" << std::endl;	
			return true;
		}
	return false;
}

bool Channel::HasPass() {
	return !pass.empty();
}

bool Channel::isUserOnChannel(User &c) const {
	if (hasMember(c) || hasOperator(c))
		return true;
	return false;
}