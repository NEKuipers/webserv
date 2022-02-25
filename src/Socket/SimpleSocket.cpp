#include "SimpleSocket.hpp"

// Default constructor
SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface) 
{
    //Define address struct
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);

    //Establish socket and connection, and test
    sock = socket(domain, service, protocol);
    test_connection(sock);
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

// Function to test sockets in connections
void    SimpleSocket::test_connection(int item_to_test)
{
    if (item_to_test < 0) {
        perror("Failed to connect...");
        exit(EXIT_FAILURE);
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
