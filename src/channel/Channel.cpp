#include "../../inc/channel/Channel.hpp"
#include <cstddef>
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
	this->_key = other._key;
	return *this;
}

void Channel::addUser(User& user) {
	_members.push_back(&user);
}

void Channel::addOps(User& user) {
	_operators.push_back(&user);
}

void Channel::removeUser(User& user) {
	for (std::vector<User *>::iterator it = _members.begin(); it != _members.end(); ++it) {
		if (*it == &user) {
			_members.erase(it);
			break;
		}
	}
	for (std::vector<User *>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
		if (*it == &user) {
			_operators.erase(it);
			break;
		}
	}
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

bool Channel::hasKey() const {
	return !_key.empty();
}

bool Channel::isOps(User &user) {
	for (size_t i = 0; i < _operators.size(); i++) {
		if (_operators[i]->getFd() == user.getFd())
			return true;
	}
	return false;
}

bool Channel::isUserOnChannel(User &c) const {
	if (hasMember(c) || hasOperator(c))
		return true;
	return false;
}