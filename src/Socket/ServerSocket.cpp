#include "ServerSocket.hpp"

// Constructor
ServerSocket::ServerSocket(int domain, int service, int protocol, int port, u_long interface, int bklg) : SimpleSocket(domain, service, protocol, port, interface)
{
	backlog = bklg;
	connect_to_network(get_sock(), get_address());
	test_connection(get_sock());
	listen(get_sock(), backlog);
}

// Definition of connect_to_network virtual function
int	ServerSocket::connect_to_network(int sock, struct sockaddr_in address)
{
	return bind(sock, (struct sockaddr *)&address, sizeof(address));
}
