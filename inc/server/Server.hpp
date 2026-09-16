#ifndef _server_hpp_
#define _server_hpp_

#include <vector>
#include <string>
#include <netinet/in.h>
#include <map>
#include "../user/User.hpp"

class Server {
private:

	int							_port;
	int							uf;
	std::string 				_password;
	int							_serverFd; // > listening socket
	struct sockaddr_in			_serverAddress;
	std::string					_hostname;
	std::vector<struct pollfd>	_pollFds; // -> active socket list
	std::map<int, User>			_users; 
	// std::vector<Channel *>	_channels; 
public:
	Server(int port, std::string password);
	~Server(void);

	void						shutdown(void);
	// Main loop
	void						startServ(void);
	// Events
	void						handleNewConnection(void);
	void						handleMessage();

	//cmd
	void invite(std::vector<std::string> args);
	void kick(std::vector<std::string> args);
	void mode(std::vector<std::string> args);
	void topic(std::vector<std::string> args);
	void join(std::vector<std::string> args);
	void privmsg(std::vector<std::string> args);
	void nick(std::vector<std::string> args);
};

#endif
