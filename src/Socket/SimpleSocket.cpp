#include "SimpleSocket.hpp"
#include <vector>
#include <unistd.h>

// Default constructor

static std::vector<int> g_open_sockets;

SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface) 
{
    //Define address struct
    address.sin_family = domain;
    address.sin_port = port;
    address.sin_addr.s_addr = interface;

    //Establish socket and connection, and test
    sock = socket(domain, service, protocol);
    g_open_sockets.push_back(sock);
    test_connection(sock);
}

SimpleSocket::SimpleSocket(struct sockaddr_in address, int sock) : address(address), sock(sock)
{}

SimpleSocket::~SimpleSocket()
{
	// usleep(3500);
	close(get_sock());
}

// Copy constructor
SimpleSocket::SimpleSocket(const SimpleSocket &src)
{
    *this = src;
}

// Assignment operator overload
SimpleSocket    &SimpleSocket::operator=(const SimpleSocket &rhs)
{
    this->address = rhs.address;
    this->sock = rhs.sock;
    return (*this);
}

void    SimpleSocket::close_all()
{
    for(std::vector<int>::const_iterator it = g_open_sockets.begin(); it != g_open_sockets.end(); it++)
        close(*it);
}

// Function to test sockets in connections
void    SimpleSocket::test_connection(int item_to_test)
{
    if (item_to_test < 0) 
    {
        throw ConnectionErrorException();
    }
}

// Getter functions
struct sockaddr_in  SimpleSocket::get_address()
{
    return this->address;
}

int                 SimpleSocket::get_sock()
{
    return this->sock;
}
