#ifndef BINDINGSOCKET_HPP
# define BINDINGSOCKET_HPP
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "SimpleSocket.hpp"

class BindingSocket: public SimpleSocket
{
	public:
		// Constructor
		BindingSocket(int domain, int service, int protocol, int port, u_long interface);

		// Virtual function from parent
		int		connect_to_network(int sock, struct sockaddr_in address);
};

#endif
