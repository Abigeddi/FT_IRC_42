#include "../Classes/Server.hpp"

Server:: Server()
{
    Message();
}
Server:: ~Server()
{
}
Server:: Server(int port, std:: string password): port(port), password(password)
{
}
Server:: Server(std:: string localhost, int port, std:: string password)
{
    this->localhost  = localhost;
    this->port = port;
    this->password = password;
}

int Server:: get_sockfd(void)
{
    return (this->sockfd);
}

int Server:: get_client_length(void)
{
    return (this->client_lenght);
}

int Server:: get_Port(void)
{
    return (this->port);
}

std:: string Server:: get_Password(void)
{
    return (this->password);
}

int Server:: get_new_socket_fd(void)
{
    return (this->new_socket_fd);
}

void Server:: create_socket(void)
{
    int reuseport = 1;
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd == -1)
        std:: cout << "Error: socket not created" << std:: endl;
    setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEPORT, &reuseport, sizeof(reuseport));
}

void Server:: bind_socket(void)
{
    memset((char*)&this->socker_addr, 0, sizeof(this->socker_addr));
    this->socker_addr.sin_family = AF_INET;
    this->socker_addr.sin_port = htons(this->port);
    this->socker_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(this->sockfd, (struct sockaddr *)&this->socker_addr, sizeof(this->socker_addr)) < 0)
    {
        std:: cout << "Failed to Bind the socket." << std:: endl;
        close (this->sockfd);
    }
}

void Server:: listen_socket(void)
{
    if (listen(this->sockfd, 5) < 0)
    {
        std:: cout << "Error: Failed to listen on socket." << std:: endl;
        close (this->sockfd);
    }
}

void Server:: accept_socket(void)
{
    size_t size;
    int num_fds;
    int sock;
    int check;
    std:: vector<std:: string> new_user;

    size = sizeof(this->socker_addr);
    memset(this->fds, 0, max_num_fds * sizeof(struct pollfd));
    this->fds[0].fd = this->sockfd;
    this->fds[0].events = POLLIN;
    num_fds = 1;
    while (true)
    {
        int ret = poll(this->fds, num_fds, -1);
        if (ret < 0)
        {
            std:: cout << "Error: Poll not Created" << std:: endl;
            exit (-1);
        }
        for (int i = 0; i < num_fds; i++)
        {
            if (this->fds[i].revents & POLLIN)
            {
                if (this->fds[i].fd == this->sockfd)
                {
                    this->new_socket_fd = accept(this->sockfd, (struct sockaddr *)&this->socker_addr, (socklen_t *)&size);
                    if (this->new_socket_fd < 0)
                    {
                        std:: cout << "Failed to accept incoming connection\n" << std:: endl;
                        close (this->sockfd);
                        exit(0);
                    }
                    if (num_fds >= max_num_fds)
                        close (this->new_socket_fd);
                    else
                    {
                        char host[256];
                        int result = getnameinfo((struct sockaddr*) &this->socker_addr, sizeof(this->socker_addr), host, 256, NULL, 0, 0);
                        std:: cout << "connection accepted" << std:: endl;
                        this->fds[num_fds].fd = this->new_socket_fd;
                        this->fds[num_fds].events = POLLIN;
                        num_fds++;
                        Message my_message(this->new_socket_fd);
                        my_message.set_time();
                        if (result == 0) {
                            my_message.add_hostname(std::string(host));
                        }
                        this->file_vectors[new_socket_fd] = my_message;
                    }
                }
                else
                {
                    sock = this->fds[i].fd;
                    read_write_socket(sock, &num_fds, &this->file_vectors[sock]);
                    check = HandleError(this->file_vectors[sock].send_Message_identification(), sock);
                }
            }
        }
    }
}

void Server:: read_write_socket(int sockfd, int *num_fds, Message *new_user)
{
    int n;
    int check;
    char buffer[511];
    std:: string tab;

    bzero(buffer, 511);
    n = recv(sockfd, buffer, 510, 0);
    buffer[n] = 0;
    check = 0;
    if (n < 0)
    {
        std:: cout << "Error: Reading From Socket" << std:: endl;
        exit(1);
    }
    if (n == 0 || !strcmp(buffer, "QUIT Leaving...\r\n"))
    {
        (*num_fds)--;
        n = HandleError(check = 11, sockfd);
        return ;
    }
    tab = buffer;
    if (check_ctrl_D(tab))
        check = new_user->parse_message(this->password, this->_buffer, *this);
    n = HandleError(check, sockfd);
    if (n < 0)
    { 
        std:: cout << "Error: Writing From Socket" << std:: endl;
        exit(1);
    }
    check_Nick_send_message(sockfd, new_user);
}

void Server:: check_Nick_send_message(int sock, Message *new_user)
{
    int check;
    int num;
    
    std:: map<int, Message>:: iterator it = this->file_vectors.find(sock);
    std:: map<int, Message>:: iterator it_incre = this->file_vectors.begin();
    if (it != this->file_vectors.end() && it != it_incre)
    {
        if (new_user->get_my_user().size() != 0)
        {
            if (it_incre != this->file_vectors.end())
            {
                for (; it_incre != it; ++it_incre)
                {
                    Message value = it_incre->second;
                    if (value.get_my_user() == new_user->get_my_user())
                    {
                        std:: string _message = "436 ERR_NICKCOLLISION " + new_user->get_my_user() +  " :Nickname collision KILL\r\n";
                        num = display_message(new_user->get_socket(), _message);
                        new_user->erase_user();
                        return ;
                    }
                }
            }
        }
    }
    check = HandleError(new_user->send_Message_identification(), sock);  
}

void Server:: send_socket(void)
{
    if (send(this->new_socket_fd, this->message, strlen(this->message), 0) < 0)
    {
        std:: cout << "Error: Message don't send" << std:: endl;
    }
}

int Server:: HandleError(int error_replies, int sockfd)
{
    int num = 0;
    std:: string handle_message;
    std:: string _message;

    switch (error_replies)
    {
        case 10:
            std:: cout << "Not Numeric" << std:: endl;
            break;
        case 11:
            close_socket(this->file_vectors[sockfd].get_socket());
            break;
        case 13:
            num = write_long_message(sockfd);
            break;
        case 14:
            num = send_private_message(sockfd);
            break;
        case 401:
            num = write(sockfd, "401 ERR_NOSUCHNICK :No such nick\r\n", 34);
            break;
        case 403:
            num = write(sockfd, "403 ERR_NOSUCHCHANNEL :No such channel\r\n", 40);
            break;
        case 404:
            num = write(sockfd, "404 ERR_CANNOTSENDTOCHAN :Cannot send to channel\r\n", 50);
            break;
        case 412:
            num = write(sockfd, "412 ERR_NOTEXTTOSEND :No text to send\r\n", 39);
            break;
        case 421 :
            num = write(sockfd, "421 ERR_UNKNOWNCOMMAND :Unknown command\r\n", 41);
            break ;
        case 431:
            num = write(sockfd, "431 ERR_NONICKNAMEGIVEN:No nickname given\r\n", 43);
            break;
        case 432:
            _message = "432 ERR_ERRONEUSNICKNAME " + this->file_vectors[sockfd].get_my_user() +  " :Erroneous nickname\r\n";
            num = display_message(sockfd, _message);
            break;
        case 436:
            _message = "436 ERR_NICKCOLLISION " + this->file_vectors[sockfd].get_my_user() +  " :Nickname collision KILL\r\n";
            num = display_message(sockfd, _message);
            break;
        case 443:
            num = write(sockfd, "443 ERR_USERONCHANNEL:user already on channel\r\n", 47);
            break;
        case 451:
            num = write(sockfd, "451 ERR_NOTREGISTERED:You have not registered\r\n", 47);
            break;
        case 464:
            num = write(sockfd, "464 ERR_PASSWDMISMATCH:Password incorrect\r\n", 43);
            break;
        case 461:
            num = display_error(sockfd);
            break;
        case 462:
            num = write(sockfd, "462 ERR_ALREADYREGISTRED USER :Unauthorized command (already registered)\r\n", 74);
            break;
        case 471:
            num = write(sockfd, "471 ERR_CHANNELISFULL <channel> :Cannot join channel (+l)\r\n",59);
            break;
        case 472:
            num = write(sockfd, "472 ERR_UNKNOWNMODE :is unknown mode char to me for this channel\r\n",66);
            break ;
        case 474:
            num = write(sockfd, "474 ERR_BANNEDFROMCHAN <channel> :Cannot join channel (+b)\r\n",60);
            break;
        case 482:
            num = write(sockfd, "482 ERR_CHANOPRIVSNEEDED You're not channel operator\r\n", 54);
            break;
        default:
            break;
    }
    return (num);
}

void Server:: close_socket(int socket)
{
    std::map<int , Message>::iterator it;

    for (it = this->file_vectors.begin(); it != this->file_vectors.end(); it++){
        if (it->second.get_socket() == socket){
            remove_user_form_channels(it->second.get_socket(), it->second.get_client().get_nick_name());
            break ;
        }
    }
    std:: cout << "Client is DISCONNECTED" << std:: endl;
    close(socket);
    this->file_vectors.erase(my_place);
    it = file_vectors.find(socket);
    if (it != file_vectors.end())
        this->file_vectors.erase(it);
}

int Server:: write_long_message(int sockfd)
{
    int num;

    num = 0;
    std:: string message;

    message = this->file_vectors[sockfd].get_welcome_message() + "\r\n";
    if (message.size() != 0)
        num = display_message(sockfd, message);
    message = this->file_vectors[sockfd].get_host_message() + "\r\n";
    if (message.size() != 0)
        num = display_message(sockfd, message);
    message = this->file_vectors[sockfd].get_server_message() + "\r\n";
    if (message.size() != 0)
        num = display_message(sockfd, message);
    
    return (num);
}

int Server:: send_private_message(int sockfd)
{
    int num = 0;
    std:: string message;

    for (size_t i = 0; i != this->file_vectors.size(); i++)
    {
        if (this->file_vectors[i].get_my_user() == this->file_vectors[sockfd].get_user_to_send())
        {
            message = ":" + this->file_vectors[sockfd].get_my_user() + this->file_vectors[sockfd].full_host() + this->file_vectors[sockfd].get_notice_private() + this->file_vectors[i].get_my_user() + " :" + this->file_vectors[sockfd].get_message_to_send() + "\r\n";
            num = display_message(this->file_vectors[i].get_socket(), message);
            return (num);
        }
    }
    message = "401 ERR_NOSUCHNICK " + this->file_vectors[sockfd].get_user_to_send() + " :No such nick/channel" + "\r\n";
    num = display_message(this->file_vectors[sockfd].get_socket(), message);
    return num;
}

int Server:: display_message(int sockfd, std:: string message)
{
    int num;

    num = 0;
    for (size_t i = 0; i < message.size(); i++)
    {
        num = write(sockfd, &message[i], 1);
        if (num < 0)
            return (-1);
    }
    return (num);
}

int Server:: display_error(int sockfd)
{
    std:: string handle_message;
    std:: string message;
    int num;

    if (this->file_vectors[sockfd].get_command().find('\n') != std:: string:: npos)
        handle_message = this->file_vectors[sockfd].get_command().substr(0, this->file_vectors[sockfd].get_command().size() - 2);
    else
        handle_message = this->file_vectors[sockfd].get_command();
    message = "461 ERR_NEEDMOREPARAMS " + handle_message + " :Not enough parameters" + "\r\n";
    num = display_message(this->file_vectors[sockfd].get_socket(), message);
    return (num);
}

bool Server:: check_ctrl_D(std:: string buffer)
{
    this->old_buffer += buffer;
    if (buffer.find("\n") != std:: string:: npos && buffer.find("\r") != std:: string:: npos)
    {
        this->_buffer = this->old_buffer;
        this->old_buffer.erase();
        return (true);
    }
    return (false);
}

Channel& Server::get_channel(std::string channel_name) {
    return (this->channels[channel_name]);
}

void    Server::add_new_channel(Channel& new_channel) {
    this->channels.insert(std::pair<std::string, Channel>(new_channel.get_channel_name(), new_channel));
}

bool    Server::channel_exists(std::string channel) {
    if (this->channels.count(channel))
        return (true);
    return (false);
}

void    Server::add_user_to_channel(int user, std::string channel) {
    this->channels[channel].add_user_to_list(user);
    if (this->channels[channel].have_an_invite(user))
        this->channels[channel].remove_user_to_invite_qeue(user);
}

void    Server::send_channel_users_list(std::string channel_name, Message& client) {
    std::map <int, Message>::iterator it;
    std::vector<int> list;
    std::string list_msg;
    std::string end_list_msg;

    end_list_msg = ": 366 " + client.get_client().get_nick_name() + " = " + channel_name + " :End of /NAMES list." + "\r\n";
    list_msg = ":irc_server 353 " + client.get_client().get_nick_name() + " = #" + channel_name + " :";
    list = this->channels[channel_name].get_users_list();
    for (it = this->file_vectors.begin(); it != this->file_vectors.end(); it++) {
        if (find(list.begin(), list.end(), it->second.get_socket()) != list.end()){
            if (this->channels[channel_name].is_admin(it->second.get_socket()))
                list_msg += "@" + it->second.get_client().get_nick_name() + " ";
            else
                list_msg += it->second.get_client().get_nick_name() + " ";
        }
    }
    list_msg += "\r\n";
    this->send_a_message(client.get_socket(), list_msg);
    this->send_a_message(client.get_socket(), end_list_msg);
}

void    Server::send_message_to_channel(std::string channel_name,std::string message, std::string client, int socket) {
    std::map<int, Message> ::iterator it;
    std::vector<int> list;
    std::string msg;

    list = this->channels[channel_name].get_users_list();
    if (message[0] == '#')
        message = message.substr(message.find(' '), message.find('\r'));
    msg = ":" + client + this->file_vectors[get_user_socket(client)].full_host() + " PRIVMSG #" + channel_name + " :" + message + "\r\n";
    for (it = this->file_vectors.begin(); it != this->file_vectors.end(); it++)
    {
        if (find(list.begin(), list.end(), it->second.get_socket()) != list.end() && it->second.get_socket() != socket)
           if( send (it->second.get_socket(),msg.c_str(),msg.size(),0) < 0)
                std::cout << "Error:  micaje not sind" << std::endl;
    }
}

void    Server::send_leave_message_to_channel(std::string channel_name, std::string client) {
    std::map<int, Message> ::iterator it;
    std::vector<int> list;
    std::string msg;

    list = this->channels[channel_name].get_users_list();
    msg = ":" + client + " PART #" + channel_name + "\r\n";
    for (it = this->file_vectors.begin(); it != this->file_vectors.end(); it++)
    {
        if (find(list.begin(), list.end(), it->second.get_socket()) != list.end() && it->second.get_client().get_nick_name() != client)
           if( send (it->second.get_socket(),msg.c_str(),msg.size(),0) < 0)
                std::cout << "Error:  micaje not sind" << std::endl;
    }
}

void    Server::send_join_message(std::string username, std::string channel_name) {
    std::map<int, Message>::iterator it;
    std::vector<int> list;
    std::string msg;
    std::string join_message;

    list = this->channels[channel_name].get_users_list();
    msg = ":" + username + this->file_vectors[get_user_socket(username)].full_host() + " JOIN #" + channel_name + "\r\n";
    for (it = this->file_vectors.begin(); it != this->file_vectors.end(); it++)
    {
        if (find(list.begin(), list.end(), it->second.get_socket()) != list.end()) {

           if( send (it->second.get_socket(),msg.c_str(),msg.size(),0) < 0)
                std::cout << "Error:  micaje not sind" << std::endl;
        }
    }
}

void    Server::send_a_message(int socket, std::string message) {
    if (send(socket, message.c_str(), message.size(), 0) < 0)
        std::cout << "couldn't send message." << std::endl;
}

void    Server::send_mode_message(std::string channel_name, std::string mode, int socket) {
    std::string msg;

    msg = ":1337_irc_server MODE " + channel_name + " " + mode + "\r\n";
    send_a_message(socket, msg);
}

std::string Server::get_channel_password(std::string channel_name) {
    return (this->channels[channel_name].get_channel_password());
}

bool Server::user_exist_in_channel(int user, std::string channel_name) {
    std::vector <int> list;

    if (user == -1)
        return (false);
    if (channel_exists(channel_name))
    {
        list = this->channels[channel_name].get_users_list();
        if (find(list.begin(), list.end(), user) != list.end()) {
            return (true);
        }
    }
    return (false);
}

void    Server::remove_user_from_channel(int user, std::string channel_name) {
    if (user == -1)
        return ;
    if (this->channels[channel_name].get_users_list().empty())
        return ;
    this->channels[channel_name].remove_user_from_channel_list(user);
}

void    Server::send_part_message_to_channel(std::string channel_name,std::string message, std::string client) {
    std::map<int, Message> ::iterator it;
    std::vector<int> list;
    std::string msg;

    list = this->channels[channel_name].get_users_list();
    if (message.empty())
        msg = ":" + client + this->file_vectors[get_user_socket(client)].full_host() + " PART #" + channel_name + "\r\n";
    else
        msg = ":" + client + this->file_vectors[get_user_socket(client)].full_host() + " PART #" + channel_name + " :" + message + "\r\n";
    for (it = this->file_vectors.begin(); it != this->file_vectors.end(); it++)
    {
        if (find(list.begin(), list.end(), it->second.get_socket()) != list.end()){

           if (send(it->second.get_socket(),msg.c_str(),msg.size(),0) < 0)
                std::cout << "Error:  message not send" << std::endl;
        }
    }
}

void    Server::send_kick_message_to_channel(std::string channel_name, std::string kicked_user, std::string reason, std::string kicker){
    std::string message;
    std::vector<int> list;
    std::map<int, Message>::iterator it;

    list = this->channels[channel_name].get_users_list();
    if (!reason.empty())
        message = ":" +  kicker + this->file_vectors[get_user_socket(kicker)].full_host() + " KICK #" + channel_name + " " + kicked_user + " :" + reason + "\r\n";
    else
        message = ":" +  kicker + this->file_vectors[get_user_socket(kicker)].full_host() + " KICK #" + channel_name + " " + kicked_user + "\r\n";
    for (it = this->file_vectors.begin(); it != this->file_vectors.end(); it++) {
        if (find(list.begin(), list.end(), it->second.get_socket()) != list.end()){
            send_a_message(it->second.get_socket(), message);
        }
    }    
}

bool    Server::is_admin(std::string channel_name, int user) {
    return (this->channels[channel_name].is_admin(user));
}

void    Server::send_channels_list(int socket, std::string search, std::string user) {
    std::map<std::string, Channel>::iterator it;
    std::string message;

    if (search.empty()) {
        for (it = this->channels.begin(); it != this->channels.end(); it++) {
            message = ":irc_server 322 " + user + " #" +  it->second.get_channel_name() + " " + std::to_string(it->second.get_users_list().size()) + "\r\n";
            send_a_message(socket, message);
        }
    }
    else {
        for (it = this->channels.begin(); it != this->channels.end(); it++) {
            if (it->second.get_channel_name().find(search) != std::string::npos) {
                message = ":irc_server 322 " + user + " #" +  it->second.get_channel_name() + " " + std::to_string(it->second.get_users_list().size()) + "\r\n";
                send_a_message(socket, message);
            }
        }
    }
}

// ":<server> TOPIC <channel> :<new topic>"
void    Server::send_topic_message(std::string channel, std::string topic) {
    std::string message;
    std::map<int, Message>::iterator it;
    std::vector<int>    users_list;

    users_list = this->channels[channel].get_users_list();
    message = ":irc_server TOPIC #" + channel + " :" + topic + "\r\n";
    for (it = this->file_vectors.begin(); it != this->file_vectors.end(); it++) {
        if (find(users_list.begin(), users_list.end(), it->second.get_socket()) != users_list.end())
            send_a_message(it->second.get_socket(), message);
    }
}

void    Server::send_topic_message_for_new_members(int socket, std::string channel) {
    std::string message;

    message = message = ":irc_server TOPIC #" + channel + " :" + this->channels[channel].get_topic() + "\r\n";
    send_a_message(socket, message);
}

void    Server::set_topic_to_channel(std::string channel, std::string topic) {
    if (channel_exists(channel))
        this->channels[channel].set_topic(topic);
}

void    Server::send_invite_message(std::string reciever, std::string sender, std::string channel) {
    std::map<int, Message>::iterator    it;
    std::string                         message;

    message = ":" + sender + " INVITE " + reciever + " #" + channel + "\r\n";
    for (it = this->file_vectors.begin(); it != this->file_vectors.end(); it++) {
        if (it->second.get_client().get_nick_name() == reciever)
            send_a_message(it->second.get_socket(), message);
    }
}

void    Server::print_current_time(int  socket){
    time_t tmn = time(NULL);
    tm* local_time = localtime(&tmn);
    std::string message;

    message = ":BOT NOTICE TIME : " + std::to_string(local_time->tm_hour) + ":" + std::to_string(local_time->tm_min) + ":" + std::to_string(local_time->tm_sec) + "\r\n";
    send_a_message(socket, message);
}


void    Server::remove_user_form_channels(int client, std::string client_name) {
    std::map<std::string, Channel>::iterator it;

    for (it = this->channels.begin(); it != this->channels.end(); it++) {
        if (user_exist_in_channel(client, it->second.get_channel_name())) {
            send_leave_message_to_channel(it->second.get_channel_name(), client_name);
            if (it->second.is_admin(client))
                it->second.remove_admin(client);
            remove_user_from_channel(client, it->second.get_channel_name());
        }
    }
}

void    Server::send_notice_message_to_channel(std::string channel_name, std::string message, std::string client) {
    std::map<int, Message>::iterator it;
    std::vector<int> users_list;
    std::string                     msg;

    msg = ":" + client + " NOTICE #" + channel_name + " :" + message + "\r\n";
    users_list = this->channels[channel_name].get_users_list();
    for (it = this->file_vectors.begin(); it != this->file_vectors.end(); it++) {
        if (find(users_list.begin(), users_list.end(), it->second.get_socket()) != users_list.end())
            send_a_message(it->second.get_socket(), msg);
    }
}

int Server::get_user_socket(std::string name) {
    std::map<int, Message>::iterator it;

    for (it = this->file_vectors.begin(); it != this->file_vectors.end(); it++) {
        if (it->second.get_client().get_nick_name() == name)
            return (it->first);
    }
    return (-1);
}
