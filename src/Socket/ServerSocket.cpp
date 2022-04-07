#include "ServerSocket.hpp"
#include <unistd.h>

// Constructor
ServerSocket::ServerSocket(int domain, int service, int protocol, int port, u_long interface, int bklg) : SimpleSocket(domain, service, protocol, port, interface)
{
	backlog = bklg;
	int yes = 1;
	if (setsockopt(get_sock(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		close(get_sock());
		throw SetSockOptErrorException(); 
	}
	connect_to_network(get_sock(), get_address());
	test_connection(get_sock());
	fcntl(get_sock(), F_SETFL, O_NONBLOCK);
	listen(get_sock(), backlog);
}

// Definition of connect_to_network virtual function
int	ServerSocket::connect_to_network(int sock, struct sockaddr_in address)
{
	return bind(sock, (struct sockaddr *)&address, sizeof(address));
}
