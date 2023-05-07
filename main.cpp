#include "Classes/Server.hpp"
#include "Classes/Message.hpp"
#include "Classes/Client.hpp"
#include <netdb.h>

int main(int ac, char **av)
{
    int i;
    std:: string password;

    i = 0;
    if (ac != 3)
    {
        std:: cout << "Must have 3 args" << std:: endl;
        return (-1);
    }
    password = av[2];
    while (av[1][i])
    {
        if (!isdigit(av[1][i]))
        {
            std:: cout << "it most have digit" << std:: endl;
            return (-1);
        }
        i++;
    }
    if (atoi(av[1]) <= 6660 || atoi(av[1]) >= 6669)
        return (std:: cout << "Error : port must be between 6660 and 6669", -1);
    if (password.size() <= 2)
    {
        std:: cout << "Error: password must be have at least 3 characters" << std:: endl;
        return (-1);
    }
    Server server(atoi(av[1]), password);
    server.create_socket();
    server.bind_socket();
    server.listen_socket();
    server.accept_socket();
    return (0);
}
