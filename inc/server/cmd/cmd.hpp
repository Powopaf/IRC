#ifndef CMD_HPP
#define CMD_HPP

#include <string>
#include <vector>

void invite(std::vector<std::string> args);
void kick(std::vector<std::string> args);
void mode(std::vector<std::string> args);
void topic(std::vector<std::string> args);
void join(std::vector<std::string> args);
void privmsg(std::vector<std::string> args);
void nick(std::vector<std::string> args);

#endif