#pragma once

#include "Server.hpp"

//feel free to make any change you want there


class User{
	private: 
		int 					_fd;
		bool 					_registered;
		bool 					_isadmin;
		bool 					_isloggedin;
		std::string             _nickname;
		std::string             _username;
		std::string             _ipv4_addr;
		std::string             _buffer;
    public:
		User();
		~User();
};