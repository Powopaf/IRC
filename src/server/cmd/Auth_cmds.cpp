#include "../../../inc/server/Server.hpp"
#include "../../../inc/server/cmd/cmd.hpp"

void    Server::handlePasswordAuth(std::vector<std::string> args){
    if (args.size() != 2){
        sendResponse("Error: PASS needs one argument only\r\n");
        return;
    }
    if (_password != args[1]){
        sendResponse("Error: Wrong password\r\n");
        return;
    }
    _users[uf].setGotPass();
}

void    Server::handleNick(std::vector<std::string> args){
    if (args.size() != 2){
        sendResponse("Error: NICK needs one argument only\r\n");
        return;
    }
    _users[uf].setNick(args[1]);
    _users[uf].setGotNick();
}

void    Server::handleUserName(std::vector<std::string> args){
    if (args.size() != 2){
        sendResponse("Error: USER needs one argument only\r\n");
        return;
    }
    _users[uf].setUsername(args[1]);
    _users[uf].setGotUsername();
}

void    Server::registerAttempt(){
    if (_users[uf].getHasPass() && _users[uf].getHasNick() && _users[uf].getHasUsername()) {
        _users[uf].setLogged();
        sendResponse("Congrats you have logged in to this irc server!\r\n");
    }
}