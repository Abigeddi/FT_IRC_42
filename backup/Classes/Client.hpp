#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <unistd.h>
#include <time.h>

class Client
{
private:
    std:: string nick_name;
    std:: string user_name;
    std:: string mode;
    std:: string unused;
    std:: string realname;
public:
    Client();
    ~Client();

    std::string get_nick_name();
    std::string get_user_name();
    void        set_nick(std:: string message);
    void        set_user(std:: string message);
    int         parse_nickname(std:: string vector, bool enter);
    int         parse_username(std:: string vector);
    int         check_parameters(std:: string parameters);
};

#endif