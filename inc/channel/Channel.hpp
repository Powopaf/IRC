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
		unsigned int _usersLimit; // is = 0 then there is no limit
		std::string pass;
		Channel();
	public:
	Channel(std::string name);
	~Channel() {}
	Channel(const Channel& copy);
	Channel& operator=(const Channel& other);

	void addUser(User& user);
	void addOps(User& user);

	bool HasPass();
	std::string getName() const { return _name; }
	std::string getPass() const { return pass; }

};

#endif
