#include "handle_message.hpp"
#include <cctype>
#include <cstddef>
#include <stdexcept>
#include <vector>


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
    while (std::isupper(msg[i])) {
        cmd[j] = msg[i];
        i++; j++;
    }
    if (msg[i] != ' ')
        throw std::invalid_argument("The command is invalid" + msg);
    return cmd;
}

static std::vector<std::string> extract_args(std::string msg, size_t i) {
    std::vector<std::string> args;
    while (msg[i] != '\r') {
        size_t j = 0;
        std::string a;
        while (msg[i] != ' ') {
            a[j] = msg[i];
            i++; j++;
        }
        args.push_back(a);
        i++;
    }
    return args;
}

void handle_message(const std::string& msg) {
    std::string cmd;
    std::vector<std::string> args;
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
    args = extract_args(msg, i);
}
