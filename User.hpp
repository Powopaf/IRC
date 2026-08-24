#pragma once

#include "Server.hpp"

class User{
	private: 
		int fd;
		std::string IPaddress;
    public:
		Client(){};
		int GetFd(){return Fd;}

		void SetFd(int fd){Fd = fd;}
		void setIpAdd(std::string ipadd){IPadd = ipadd;}
}