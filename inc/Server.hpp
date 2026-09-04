#ifndef _server_hpp_
# define _server_hpp_

# include <iostream>
# include <vector>
# include <map>
# include <cstdlib>
# include <stdexcept>
# include <string>
# include <cstring>
# include <cstdio>
# include <unistd.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <poll.h>
# include <fcntl.h>
# include <sstream>
# include <ctime>
# include <csignal>


class Server {
private:

	int 									_port;
	std::string 							_password;
	int 									_serverFd; // > listening socket
	struct sockaddr_in 						_serverAddress;
	std::string 							_hostname;
	std::vector<struct pollfd> 				_pollFds; // -> active socket list
	// std::vector<Client *> 					_clients; 
	// std::vector<Channel *>					_channels; 
public:
	Server(int port, std::string password);
	~Server(void);
	void 									shutdown(void);

	/* Main loop */
	void 									startServ(void);
};

#endif
