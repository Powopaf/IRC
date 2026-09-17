#ifndef CMD_HPP
#define CMD_HPP

#include <string>
#include <vector>

void invite(std::vector<std::string> args);
void kick(std::vector<std::string> args);
void mode(std::vector<std::string> args);
void topic(std::vector<std::string> args);

#endif

// class Server;

// class CmdHandler {
// private:
//     Server  &theserver; //to use server info during commands


// public:
//     CmdHandler(Server &);
//     ~CmdHandler();

//     void invite(std::vector<std::string> args);
//     void kick(std::vector<std::string> args);
//     void mode(std::vector<std::string> args);
//     void topic(std::vector<std::string> args);
// };
// #endif