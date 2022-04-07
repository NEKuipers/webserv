// This class serves as a client and is probably unnecessary for webserv, but may come in handy for testing?

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "SimpleSocket.hpp"
#include "Request.hpp"

enum read_status
{
	NOT_COMPLETE,
	HEADER_COMPLETE,
	BODY_COMPLETE
};

class ClientSocket: public SimpleSocket
{
	std::string buffer;
	Request 	request;
	bool 		headers_complete;


	public:
		// Constructor
		ClientSocket(struct sockaddr_in address, int sock);

		enum read_status	read_to_request();
		Request				get_request();
};

#endif
