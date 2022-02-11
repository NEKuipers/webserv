#include "SimpleSocket.hpp"


SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface) 
{
    //Define address struct
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    //Establish connection
    sock = socket(domain, service, protocol);

}
