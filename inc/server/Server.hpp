#ifndef _server_hpp_
#define _server_hpp_

#include <vector>
#include <string>
#include <netinet/in.h>
#include <map>
#include <iostream>
#include <exception>
#include <ostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
#include <stdexcept>
#include <arpa/inet.h>
#include <utility>
#include <csignal>
#include <cstdlib>
#include <cstdio>
#include "../user/User.hpp"

class Server {
private:
	// CmdHandler					_commandHandler;

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
	void 						handleDisconnection();
	void						_rmClient();
	void						handleMessage();
	void						sendResponse(std::string response);
	int							_getUF() { return (uf); }
	std::map<int, User>			getUsers() { return _users; }
	//Executing

	void						handlePasswordAuth(std::vector<std::string> args); 
	void						handleNick(std::vector<std::string> args);
	void						handleUserName(std::vector<std::string> args);
	void						registerAttempt();

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
