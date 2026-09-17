#include "../../inc/channel/Channel.hpp"

Channel::Channel() {

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