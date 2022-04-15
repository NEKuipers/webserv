// This class serves as a client and is probably unnecessary for webserv, but may come in handy for testing?

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "SimpleSocket.hpp"
#include "Request.hpp"
#include "ConfigResponse.hpp"
#include "Response.hpp"

class ClientSocket: public SimpleSocket
{
	private:
		std::string 		buffer;
		Request 			request;
		bool 				headers_complete;
		struct sockaddr_in	socket_address;
		Response			*http_response;

	public:
		ConfigResponse*		conf_response;

		// Constructor
		ClientSocket(struct sockaddr_in address, int sock, struct sockaddr_in sock_adr);
		virtual ~ClientSocket();
		void 				createResponse();
		Request				get_request();
		Response			*get_http_response() const;
		
		void				read(const std::string& read);
		bool				check_body();
		bool				check_headers();
				
};

#endif
