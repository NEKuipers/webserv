// This class serves as a client and is probably unnecessary for webserv, but may come in handy for testing?

#ifndef CONNECTINGSOCKET_HPP
# define CONNECTINGSOCKET_HPP
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "SimpleSocket.hpp"

class ConnectingSocket: public SimpleSocket
{
	public:
		// Constructor
		ConnectingSocket(int domain, int service, int protocol, int port, u_long interface);
		
		// Virtual function from parent
		int		connect_to_network(int sock, struct sockaddr_in address);
};



#endif