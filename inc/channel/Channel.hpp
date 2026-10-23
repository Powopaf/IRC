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
		Channel();
	public:
	Channel(std::string name);
	~Channel() {}
	Channel(const Channel& copy);
	Channel& operator=(const Channel& other);

	void addUser(User& user);
	void addOps(User& user);
	void addOps(std::string& nick);
	void rmOps(std::string& nick);
	void removeUser(User& user);
	bool isOps(User& user);

	bool hasKey() const;
	std::string getName() const { return _name; }
	const std::vector<User *>& getMembers() const { return _members; }
	bool hasMember(const User& user) const;
	bool hasMember(const std::string& nick) const;
	bool hasOperator(const User& user) const;
	bool isInviteOnly() const { return _inviteOnly; }
	std::string getKey() const { return _key; }
	bool getTopicRestrited() const { return _topicRestricted; }
	std::string getTopic() const { return _topic; }
	const std::vector<User*>& getUsers() const { return _members; }
	const unsigned int getLimit() const { return _usersLimit; }
	const std::vector<User*>& getOps() const { return _operators; }

	bool isUserOnChannel(User &c) const;

	void setTopic(std::string& topic) { _topic = topic; }
	void setInviteONly(bool invite) { _inviteOnly = invite; }
	void setTopicRestricted(bool restricted) { _topicRestricted = restricted; }
	void setKey(const std::string& key) { _key = key; }
	void setLimit(unsigned int n) { _usersLimit = n; }
};

#endif
