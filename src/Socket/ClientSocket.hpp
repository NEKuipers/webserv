// This class serves as a client and is probably unnecessary for webserv, but may come in handy for testing?

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "SimpleSocket.hpp"

class ClientSocket: public SimpleSocket
{
	std::string buffer;

	public:
		// Constructor
		ClientSocket(struct sockaddr_in address, int sock);

		bool			read_to_buffer();
		std::string		get_buffer();

};

#endif
