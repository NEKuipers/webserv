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
	std::string		to_write;
	ssize_t			written_size;

	public:
		ConfigResponse*	response;
		bool			read_successfully;

		// Constructor
		ClientSocket(struct sockaddr_in address, int sock);
		virtual ~ClientSocket();
		void 				createResponse();
		Request				get_request();

		bool				send();
		void				read();
		bool				check_body();
		bool				check_headers();
		
};

#endif
