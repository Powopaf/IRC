#include "Server.hpp"
#include "User.hpp"

int main(int ac, char **av){
    if (ac != 3){
	    std::cout << "Please format this way: " << av[0] << " <port number> <password>" << std::endl; return 1;}
	Server server;
	return 0;
}