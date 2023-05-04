#include "../Classes/Channel.hpp"

Channel:: Channel()
{
    set_channels_modes();
    limit_ban_list = 10;
    limit = 10;
    limite = false;
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

// int parse_mode(std:: string request, Server &server)
// {
    
// }

void    Channel::add_user_to_list(int user) {
    this->users_list.push_back(user);
}

void    Channel::print_users_list() {
    std::vector <int>::iterator it;

    for (it = this->users_list.begin(); it != this->users_list.end(); it++) {
        std::cout <<"user : " << *it << std::endl;
    }
}

std::vector<int> Channel::get_users_list() {
    return (this->users_list);
}

void    Channel::empty_channel() {
    this->name = "";
    this->password = "";
}

// int    Channel::add_admin(std::string user_nick){
//     if (user_in_channel)
//     {
//         if (!is_admin(user_nick))
//         {
//             this->admins.push_back(user_nick);
//         }
//         else
//             std::cout << "already added admin" << std::endl;    
//         return (0);
//     }
//     else
//         return (404);
// }

int    Channel::add_admin_to_list(int socket) {
    this->admins.push_back(socket);
    std::vector<int>::iterator it;

    for (it = this->admins.begin(); it != this->admins.end(); it++) {
        std::cout << "admin +============== " << *it << std::endl;
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
        std::cout << "user remouved from channel " + this->get_channel_name() << std::endl;
    }
    this->print_users_list();
}

bool    Channel::is_admin(int socket) {
    std::cout << "is admin recieved : " << socket << std::endl;
    std::cout << "recieved :: " << socket << std::endl;
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
            std::cout << "remouved ." << std::endl;
        }
        index++;
    }
}

bool    Channel::have_an_invite(int user) {
    if (find(this->invited_list.begin(), this->invited_list.end(), user) != this->invited_list.end())
        return (true);
    return (false);
}
void Channel::set_channels_modes()
{
    channel_modes.insert (std::make_pair ('o',0));
    channel_modes.insert (std::make_pair ('b',0));
    channel_modes.insert (std::make_pair ('l',0));
}

int Channel::find_modes(char c)
{
    std::map<char, bool>::iterator it;

    for (it = channel_modes.begin(); it != channel_modes.end(); it++)
    {
        if (it->first == c)
            return (1);
    }
    return (0);
}

int Channel::set_modes(std::string modes,std::string param, int socket)
{
    char cuurent_modes ;
    std::map<char, bool>::iterator it;
    size_t i = 0;
    std::cout << "PARAM IN SET MODE "<< param << std::endl;
    std::cout << "MODE  IN SET MODE "<< modes << std::endl;
    // if (param.empty())
    //     return (0);
    while (i < modes.length())
    {
        cuurent_modes =  modes[i] ;
        std::cout << "current mode :" << "["<<cuurent_modes<<"]" <<std::endl;
        if (find_modes(cuurent_modes))
        {
            for (it = channel_modes.begin(); it != channel_modes.end(); it++)
            {
                std::cout << "dkhlate hna west iterator dial modes" << std::endl;
                std::cout  << "channel mode 99900666"<<it->first << " " << it->second << std::endl;
                    std::cout << "[" << cuurent_modes << "]" << std::endl;
                if (it->first == cuurent_modes)
                {
                    if (it->second == true)
                        std::cout <<"Mode: "<< it->first << " is already set /r/n";
                    else
                    {
                        it->second = true;
                        return (execute_mode(it->first,param, socket));
                    }
                }
            }
        }
        else
            return (472);
        i++;
    }
    return (0);
}

int Channel::unset_modes(std::string modes)
{
    char cuurent_mode ;
    std::map<char, bool>::iterator it;
    size_t i = 1;
    while (i < modes.length())
    {
        cuurent_mode = modes[i];
        if (find_modes(cuurent_mode))
        {
            for (it = channel_modes.begin(); it != channel_modes.end(); it++)
            {
                if (it->first == cuurent_mode)
                {
                    std::cout << it->first << std::endl;
                    if (it->second == false)
                        std::cout << "this mode is alredy unset" << std::endl;
                    else if (it->second == true)
                        it->second = false;
                }
            }
        }
        i++;
    }
    return (0);
}

int Channel::execute_mode(char c,std::string param, int user_socket)
{
    std::cout << "dkhlate eexcute mode" << std::endl;
    switch (c)
    {
        case 'o' :
            if (!param.empty())
            {
                if (user_is_in_channels(user_socket)) {
                    this->admins.push_back(user_socket);
                    return (0);
                }
                else
                    return (std::cout <<  param << "is already admin" << std::endl,0);
            }
            else 
                return (461);
        break;
        case 'l' :
        limite = true;
        if (!param.empty())
        {
            limit  = atoi(param.c_str());
            set_limit(limit);
            return (0);
        }
        else
            return (461);
        break;
        case 'b' :
            if (!param.empty())
                return (add_to_ban_list(user_socket));
            else
                return (461);
        default :
            return (472);
            break;
    }      
    return (0);
}

void Channel::set_limit(int user_num_channel)
{
    this->limit = user_num_channel;
}

bool Channel::user_is_in_channels(int socket)
{
    if (find(this->users_list.begin(), this->users_list.end(), socket) != this->users_list.end())
        return (true);
    return (false);
}

int Channel::add_to_ban_list(int user)
{
    if (limit_ban_list <= ban_list.size())
        return (478);
    if (user_is_in_channels(user))
    {
        this->ban_list.push_back(user);
        remove_user_from_channel_list(user);
        return (0);
    }
    else
    {
       this->ban_list.push_back(user);
       return (0);
    }
}

void    Channel::remove_admin(int socket) {
    std::vector<int>::iterator it;

    if (this->admins.empty())
        return ;
    it = find(this->admins.begin(), this->admins.end(), socket);
    if (it != this->admins.end())
        this->admins.erase(it);
}

bool           Channel::is_banned(int user)
{
    std::vector<int>::iterator it;

    for (it = this->ban_list.begin(); it != this->ban_list.end();it++)
    {
        if (*it == user)
            return (true);
        std::cout << "was here to confirm is banned" << std::endl;
    }
    return (false);
}
size_t  Channel::get_limit() {
    return (this->limit);
}