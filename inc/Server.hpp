#ifndef _server_hpp_
# define _server_hpp_

# include <vector>
# include <string>
# include <unistd.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <poll.h>
# include <fcntl.h>
# include "User.hpp"

class Server {
private:

	int 						_port;
	std::string 				_password;
	int 						_serverFd; // > listening socket
	struct sockaddr_in 			_serverAddress;
	std::string 				_hostname;
	std::vector<struct pollfd>	_pollFds; // -> active socket list
	std::vector<User *> 		_users; 
	// std::vector<Channel *>	_channels; 
public:
	Server(int port, std::string password);
	~Server(void);

	void 						shutdown(void);
	// Main loop
	void 						startServ(void);
	// Events
	void 						handleNewConnection(void);
	void						handleMessage(const std::string& msg, std::string cmd, std::vector<std::string> args);

};

#endif
