#include "handle_message.hpp"


/*
IRC msg:

[:prefix] <command> <param> 	[:trailing]\r\n

must not have a size > 512
up to 15 param
command are upper letter or 3 digits
we don't care about prefix
:trailing the last param can contain space
*/


void handle_message(const std::string& msg) {
    
}
