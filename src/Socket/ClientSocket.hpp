// This class serves as a client and is probably unnecessary for webserv, but may come in handy for testing?

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "SimpleSocket.hpp"
#include "Request.hpp"
#include "ConfigResponse.hpp"

class ClientSocket: public SimpleSocket
{
	std::string 	buffer;
	Request 		request;
	bool 			headers_complete;


	public:
		ConfigResponse*	response;

		// Constructor
		ClientSocket(struct sockaddr_in address, int sock);
		virtual ~ClientSocket();

		Request				get_request();
		bool				read_body();
		bool				read_headers();
};

#endif
