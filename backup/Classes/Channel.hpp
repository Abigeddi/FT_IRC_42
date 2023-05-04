#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
# include <vector>
# include <map>

class Channel
{
private:
    std:: string name;
    std::vector <int>   admins;
    std:: string password;
    std::string mode;
    std::vector <int>   users_list;
    std::string topic;
    std::vector <int>   invited_list;
    std::map<char, bool> channel_modes ;
    size_t limit;
    bool limite;
    bool  user_in_channel;
    std::vector    <int> ban_list;
    size_t limit_ban_list;

public:
    Channel();
    ~Channel();
    int                         parse_channel(std:: string channel, Channel& msg_channel);
    std::string                 get_channel_name();
    std::string                 get_channel_password();
    void                        add_user_to_list(int user);
    void                        print_users_list();
    std::vector<int>            get_users_list();
    void                        empty_channel();
    void                        remove_user_from_channel_list(int username);
    bool                        is_admin(int socket);
    bool                        check_join_request(std::string request);
    bool                        is_empty(std::string check);
    void                        set_topic(std::string _topic);
    std::string                 get_topic();
    void                        add_user_to_invite_qeue(int user);
    void                        remove_user_to_invite_qeue(int user);
    bool                        have_an_invite(int user);
    void                        set_channels_modes();
    int                         find_modes(char c);
    int                         set_modes(std::string,std::string, int socket);
    int                         unset_modes(std::string);
    int                         execute_mode(char,std::string, int user_socket);
    void                        set_limit(int);
    bool                        user_is_in_channels(int socket);
    int                         add_to_ban_list(int);
    void                        remove_admin(int socket);
    bool                        is_banned(int);
    std::vector <std::string>   get_my_channels_list();
    int    add_admin_to_list(int socket);
    size_t                      get_limit();
};

#endif