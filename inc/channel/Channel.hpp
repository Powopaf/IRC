#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "../user/User.hpp"

class Channel {
	private:
		std::string _name;
		std::vector<User> _members;
	public:
	Channel();
	~Channel() {}
	Channel(const Channel& copy);
	Channel& operator=(const Channel& other);

};

#endif