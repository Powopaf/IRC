#ifndef USER_HPP
#define USER_HPP

#include <string>

class User{
	private: 
		int 				_fd;
		bool 				_isadmin;
		bool 				_isloggedin;
		std::string			_nickname;
		std::string			_username;
		std::string			_ipv4_addr;
		std::string			_buffer;
	public:
		User() : _isadmin(false), _isloggedin(false) {}
		~User() {}

		bool				hasCmd();

		//getters
		const std::string&	getBuf(void) const { return _buffer; }
		const std::string&	getHostname(void) const { return (_ipv4_addr); }
		const std::string&	getNick(void) const { return (_nickname); }
		const std::string&	getUsername(void) const { return (_username); }
		const std::string	getFullId(void) const { return (_nickname + "!" + _username + "@" + _ipv4_addr); }
		bool				getLogged(void) const { return (_isloggedin); }
		bool				getAdmin(void) const { return (_isadmin); }

		// setters
		void				setHostname(char * hostname) { _ipv4_addr = std::string(hostname); }
		void				setFd(int fd) { _fd = fd; }
		void				setIsAdmin(void) { _isadmin = true; }
		void				setLogged(void) { _isloggedin = true; }
		void				setNick(std::string& nick) { _nickname = nick; }
		void				setUsername(std::string& username) { _username = username; }
		void				setIpv4Addr(std::string ipv4_addr) { _ipv4_addr = ipv4_addr; }
		void				setBuf(std::string buffer) { _buffer += buffer; }

};

#endif