// This class serves as a client and is probably unnecessary for webserv, but may come in handy for testing?

#include "ConnectingSocket.hpp"

// Constructor
ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface)
{
	connect_to_network(get_sock(), get_address());
	test_connection(get_sock());
}

// Definition of connect_to_network virtual function
int ConnectingSocket::connect_to_network(int sock, struct sockaddr_in address)
{
	return connect(sock, (struct sockaddr *)&address, sizeof(address));
}
