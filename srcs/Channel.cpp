#include "../Classes/Channel.hpp"

Channel:: Channel()
{
    set_limit(10);
}

Channel:: ~Channel()
{
}

std::string Channel::get_channel_name() {
    return (this->name);
}

std::string Channel::get_channel_password() {
    return (this->password);
}

int Channel:: parse_channel(std:: string channel, Channel& msg_channel)
{
    
    std:: string tab;
    std::string pass;

    if (channel.find('#') != std::string::npos) {
        tab = channel.substr(channel.find('#') + 1);
        if (tab.find(' ') != std::string::npos && tab.find(' ') + 1 < tab.length() && tab[0] != ' ') {
            pass = tab.substr(tab.find(' ') + 1, tab.find('\r') - 1);
            tab = tab.substr(0, tab.find(' '));
            if (!is_empty(pass))
                msg_channel.password = pass;
            msg_channel.name = tab;
            return (0);
        }
        else if (tab[1] != ' '){
            if (tab.find(' ') != std::string::npos)
                tab = tab.substr(0, tab.find(' '));
            else
                tab = tab.substr(0, tab.find('\r'));
            msg_channel.name = tab;
            return (0);
        }
        else
            return (461);//Used to indicate the given channel name is invalid.
    }
    return (461);
}

void    Channel::add_user_to_list(int user) {
    this->users_list.push_back(user);
}

std::vector<int> Channel::get_users_list() {
    return (this->users_list);
}

void    Channel::empty_channel() {
    this->name = "";
    this->password = "";
}

int    Channel::add_admin_to_list(int socket) {
    this->admins.push_back(socket);
    std::vector<int>::iterator it;

    for (it = this->admins.begin(); it != this->admins.end(); it++) {
    }
    return (0);
}

void Channel::remove_user_from_channel_list(int user) {
    std::vector<int>::iterator it;

    if (this->users_list.empty()) {
        return;
    }

    it = find(this->users_list.begin(), this->users_list.end(), user);
    if (it != this->users_list.end()) {
        this->users_list.erase(it);
    }
}

bool    Channel::is_admin(int socket) {
    if (find(this->admins.begin(), this->admins.end(), socket) != this->admins.end())
        return (true);
    return (false);
}

bool    Channel::is_empty(std::string check) {
    for (unsigned long i = 0; i != check.length() ; i++) {
        if (check[i] != ' ')
            return (false);
    }
    return (true);
}

void        Channel::set_topic(std::string _topic) {
    this->topic = _topic;
}

std::string Channel::get_topic() {
    return (this->topic);
}

void    Channel::add_user_to_invite_qeue(int user) {
    if (user != -1)
        this->invited_list.push_back(user);
}

void    Channel::remove_user_to_invite_qeue(int user) {
    std::vector<int>::iterator it;
    size_t  index = 0;

    for (it = this->invited_list.begin(); it != this->invited_list.end(); it++)
    {
        if (this->invited_list[index] == user) {
            this->invited_list.erase(this->invited_list.begin() + index);
        }
        index++;
    }
}

bool    Channel::have_an_invite(int user) {
    if (find(this->invited_list.begin(), this->invited_list.end(), user) != this->invited_list.end())
        return (true);
    return (false);
}

bool Channel::user_is_in_channels(int socket)
{
    if (find(this->users_list.begin(), this->users_list.end(), socket) != this->users_list.end())
        return (true);
    return (false);
}

void    Channel::remove_admin(int socket) {
    std::vector<int>::iterator it;

    if (this->admins.empty())
        return ;
    it = find(this->admins.begin(), this->admins.end(), socket);
    if (it != this->admins.end())
        this->admins.erase(it);
}

void Channel::set_limit(size_t limit)
{
    this->limit = limit;
}

size_t Channel::get_limit()
{
    return (this->limit);
}