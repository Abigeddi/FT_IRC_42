#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <vector>
#include "../Classes/Server.hpp"
#include "../Classes/Client.hpp"
#include "../Classes/Channel.hpp"
# include <map>

class Server;

class Message
{
private:
    int socket;
    bool enter;
    std::string hostname;
    std:: string message;
    std:: string welcome_message;
    std:: string host_message;
    std:: string server_message;
    std:: string prefix;
    std:: string command;
    std:: string password;
    std:: string my_user;
    std:: string user_to_send;
    std:: string message_to_send;
    std:: string notice_private;
    std:: vector<std:: string> params;
    std:: vector<std:: string> Mychannels;
    Client client;
    Channel channel;
    std::vector<std::string> joined_channels;
    std::map<std::string, Channel> channels;
    time_t _time;

public :
    Message();
    Message(int socket);
    Message(std:: string message);
    ~Message();
    std:: string	get_message(void);
    std:: string	get_welcome_message(void);
    std:: string	get_host_message(void);
    std:: string	get_server_message(void);
    std:: string	get_command(void);
    std:: string	get_my_user(void);
    std:: string	get_user_to_send(void);
    std:: string	get_message_to_send(void);
    std:: string	get_password(void);
    std:: string	get_notice_private(void);
    int          	get_socket(void);
    void			set_message(std:: string message);
    std::string		handle_space(std:: string message, std:: string command);
    int				parse_message(std:: string password, std:: string message, Server& server);
    int				check_upper(std:: string command);
    int				check_my_vector(std:: string request, Server& server);
    int				check_Error_Space(std:: string command);
    int				send_Message_identification();
    int				check_Password_Space(int size, std:: string command, std:: string message, std:: string password);
    int				parse_private_message(std:: string message);
    std::vector<std::string> create_vector(void);
    bool			check_command(std:: string command);
    int				check_message(std:: string message);
    void			add_new_channel();
    int				parse_channel_message(std::string request, Server& server);
    Client 			get_client();
    int				parse_part_command(std::string request, Server& server);
    int				parse_kick_command(std::string request, Server& server);
    int				parse_list_command(std::string request, Server& server);
    void			add_a_channel_to_list(std::string channel);
    bool            check_list_param(std::string param);
    int             parse_topic(std::string request, Server& server);
    int             parse_invite_command(std::string request, Server& server);
    int             parse_bot_command(std::string request, Server& server);
    void            set_time();
    std::string     get_logtime();
    int             parse_notice_for_channel(std::string request, Server& server);
    int             check_mode (std::string mode, std::string channel_name,Server &server);
    int             parse_Mode_command(std::string request, Server& server);
    int             add_mode_to_channel(std::string,std::string,std::string,Server&);
    int             remove_mode_from_channel(std::string,std::string,std::string,Server&);
    void            add_channel_to_my_list(std::string channel);
    void            add_hostname(std::string);
    std::string     get_host_name(void);
    std::vector<std::string> get_my_channels_list();
    void            erase_user(void);
    std::string     full_host();
};

#endif
