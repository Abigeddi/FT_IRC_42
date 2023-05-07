#ifndef SERVER_HPP
# define SERVER_HPP

#include <ctime>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/poll.h>
#include <stdlib.h>
#include <map>
#include "../Classes/Channel.hpp"
#include "../Classes/Message.hpp"

# define max_num_fds 10

class Message;

class Server
{
	private:
		int								sockfd;
		int								client_lenght;
		int								num_bytes;
		int								port;
		int								my_place;
		const char						*message;
		int								new_socket_fd;
		std::string						localhost;
		std::string 					password;
		struct sockaddr_in				socker_addr;
		struct pollfd 					fds[max_num_fds];
		std:: map<int, Message> 		file_vectors;
		std:: string					_buffer;
		std:: string					old_buffer;
		std::map<std::string, Channel>	channels;

	public:
		Server();
		Server(int port, std:: string password);
		Server(std:: string localhost, int port, std:: string password);
		~Server();
		int         get_sockfd();
		int         get_client_length();
		int         get_Port();
		std::string get_Password();
		int         get_new_socket_fd();
		void        create_socket();
		void        accept_socket();
		int         read_socket();
		int         write_socket();
		void        bind_socket();
		void        listen_socket();
		void        read_write_socket(int sockfd, int *num_fds, Message *new_user);
		void        send_socket(void);
		void        close_socket(int socket);
		int         HandleError(int error_replies, int sockfd);
		void        add_User(int fd, std:: vector<std:: string> new_user);
		int         write_long_message(int sockfd);
		int         display_message(int sockfd, std:: string message);
		bool        check_ctrl_D(std:: string buffer);
		void        add_new_channel(Channel& new_channel);
		bool        channel_exists(std::string channel);
		void        add_user_to_channel(int, std::string channel);
		void        send_channel_users_list(std::string channel_name, Message& client);
		void    	send_join_message(std::string username, std::string channel_name);
		void        send_a_message(int socket, std::string message);
		void        send_mode_message(std::string channel_name, std::string channel_mode, int socket);
		std::string get_channel_password(std::string channel_name);
		bool        user_exist_in_channel(int user, std::string channel_name);
		void        remove_user_from_channel(int user, std::string channel_name);
		void        send_part_message_to_channel(std::string channel_name, std::string message, std::string client);
		void        send_kick_message_to_channel(std::string channel_name, std::string kicked_user, std::string reason, std::string kicker);
		bool        is_admin(std::string channel_name, int user);
		void        send_channels_list(int socket, std::string search, std::string user);
		Channel&	get_channel(std::string channel_name);
    	void        send_topic_message(std::string channel, std::string topic);
		void		send_topic_message_for_new_members(int socket, std::string channel);
		void		set_topic_to_channel(std::string channel, std::string topic);
		void		send_invite_message(std::string reciver, std::string sender, std::string channel);
		void    	print_current_time(int socket);
		void		remove_user_form_channels(int client, std::string client_name);
		void        send_message_to_channel(std::string channel_name, std::string message, std::string client, int socket);
		void        send_leave_message_to_channel(std::string channel_name, std::string client);
		void		send_notice_message_to_channel(std::string channel_name, std::string message, std::string client);
        int			display_error(int sockfd);
        int			send_private_message(int sockfd);
		int			get_user_socket(std::string name);
		void		check_Nick_send_message(int sock, Message *new_user);
};

#endif
