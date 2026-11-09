#include "../../../inc/server/Server.hpp"

void    Server::handlePasswordAuth(std::vector<std::string> args){
    if (args.size() != 1){
        sendResponse("Error: PASS needs one argument only\r\n");
        return;
    }
    if (_password != args[0]){
        sendResponse("Error: Wrong password\r\n");
        return;
    }
    _users[uf].setGotPass();
}

void    Server::handleNick(std::vector<std::string> args){
    if (args.size() != 1){
        sendResponse("Error: NICK needs one argument only\r\n");
        return;
    }
    _users[uf].setNick(args[0]);
    _users[uf].setGotNick();
}

void Server::handleUserName(std::vector<std::string> args) {
    if (args.size() < 4) {
        sendResponse(":" + _hostname + " 461 * USER :Not enough parameters\r\n");
        return;
    }
    _users[uf].setUsername(args[0]);
    _users[uf].setGotUsername();
}

// void    Server::handleUserName(std::vector<std::string> args){
//     if (args.size() != 1){
//         sendResponse("Error: USER needs one argument only\r\n");
//         return;
//     }
//     _users[uf].setUsername(args[0]);
//     _users[uf].setGotUsername();
// }

// void    Server::registerAttempt(){
//     if (_users[uf].getHasPass() && _users[uf].getHasNick() && _users[uf].getHasUsername()) {
//         _users[uf].setLogged();
//         sendResponse("Congrats you have logged in to this irc server!\r\n");
//     }
// }

void Server::registerAttempt() {
    User& u = _users[uf];
    if (!u.getLogged() && u.getHasPass() && u.getHasNick() && u.getHasUsername()) {
        u.setLogged();
        std::string nick = u.getNick();
        sendResponse(":" + _hostname + " 001 " + nick + " :Welcome to the IRC network, " + u.getFullId() + "\r\n");
        sendResponse(":" + _hostname + " 002 " + nick + " :Your host is " + _hostname + "\r\n");
        sendResponse(":" + _hostname + " 003 " + nick + " :This server was created today\r\n");
        sendResponse(":" + _hostname + " 004 " + nick + " " + _hostname + " 1.0 o itkol\r\n");
    }
}