#include "handle_message.hpp"
#include <cctype>
#include <cstddef>
#include <stdexcept>


/*
IRC msg:

[:prefix] <command> <param> 	[:trailing]\r\n

must not have a size > 512
up to 15 param
command are upper letter or 3 digits
we don't care about prefix
:trailing the last param can contain space
*/

static std::string extract_cmd(std::string msg, size_t i) {
    std::string cmd;
    size_t j = 0;
    if (std::isdigit(msg[i])) {
        while (std::isdigit(msg[i])) {
            cmd[j] = msg[i];
            j++; i++;
        }
        if (j != 3)
            throw std::invalid_argument("The command is invalid" + msg);
    }
    else {
        while (std::isupper(msg[i])) {
            cmd[j] = msg[i];
            i++; j++;
        }
        if (msg[i] != ' ')
            throw std::invalid_argument("The command is invalid" + msg);
    }
    return cmd;
}

void handle_message(const std::string& msg) {
    std::string cmd;
    size_t i = 0;
    if (msg.size() > 512)
        throw std::length_error("Message is too long");
    if (msg[msg.size() - 1] != '\n' && msg[msg.size() - 2] != '\r')
        throw std::invalid_argument("Message does not end with \\r\\n");
    if (msg[0] == ':') {
        while (msg[i] != ' ')
            i++;
    }
    i++;
    cmd = extract_cmd(msg, i);
}
