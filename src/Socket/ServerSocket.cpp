#include "ServerSocket.hpp"
#include <unistd.h>


// Constructor
ServerSocket::ServerSocket(int domain, int service, int protocol, int port, u_long interface, int bklg) : SimpleSocket(domain, service, protocol, port, interface)
{
	this->sock_fd = get_sock();
	backlog = bklg;
	int yes = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		close(sock_fd);
		throw ConnectionErrorException(); //TODO change later
	}
	connect_to_network(get_sock(), get_address());
	test_connection(get_sock());
	fcntl(get_sock_fd(), F_SETFL, O_NONBLOCK);
	listen(get_sock(), backlog);
}

// Definition of connect_to_network virtual function
int	ServerSocket::connect_to_network(int sock, struct sockaddr_in address)
{
	return bind(sock, (struct sockaddr *)&address, sizeof(address));
}

void	ServerSocket::set_sock_fd(int newsock)
{
	this->sock_fd = newsock;
}

int 	ServerSocket::get_sock_fd()
{
	return this->sock_fd;
}
