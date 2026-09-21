#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "../user/User.hpp"

class Channel {
	private:
		std::string _name;
		std::string _topic;
		std::vector<User *> _members;
		std::vector<User *> _operators;
		std::string _key;
		bool _inviteOnly;
		bool _topicRestricted;
		unsigned int _usersLimit;
	public:
	Channel();
	~Channel() {}
	Channel(const Channel& copy);
	Channel& operator=(const Channel& other);

	void addUser(User& user);

	std::string getName();

};

#endif
