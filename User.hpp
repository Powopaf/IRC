#pragma once

#include "Server.hpp"

class User{
	private: 
		int fd;
		bool registered;
		bool isadmin;
		bool isloggedin;
		std::string ipadd;
		std::string username;
		std::string nickname;
    public:
		User();
		User(std::string nickname, std::string username);
		~User();
		User(User const &src);
		User &operator=(User const &src);
		std::string GetNickName();
		std::string GetUserName();
		bool 		GetLogedIn();
		std::string getIpAdd();
		
};