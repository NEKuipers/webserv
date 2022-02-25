#include "BindingSocket.hpp"

// Constructor
BindingSocket::BindingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface)
{
	connect_to_network(get_sock(), get_address());
	test_connection(get_sock());
}

// Definition of connect_to_network virtual function
int	BindingSocket::connect_to_network(int sock, struct sockaddr_in address)
{
	return bind(sock, (struct sockaddr *)&address, sizeof(address));
}
