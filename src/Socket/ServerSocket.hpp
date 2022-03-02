#ifndef ServerSocket_HPP
# define ServerSocket_HPP
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "SimpleSocket.hpp"

class ServerSocket: public SimpleSocket
{
	private:
		int backlog; //The maximum amount of queued connections
		int sock_fd;
	public:
		// Constructor
		ServerSocket(int domain, int service, int protocol, int port, u_long interface, int bklg);

		// Virtual function from parent
		int		connect_to_network(int sock, struct sockaddr_in address);

		void	set_sock_fd(int newsock);
		int 	get_sock_fd();
};

#endif
