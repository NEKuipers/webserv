#include "ClientSocket.hpp"
#include "Request.hpp"
#define BUFFER_SIZE 30000
#include <unistd.h>

// Constructor
ClientSocket::ClientSocket(struct sockaddr_in address, int sock) : SimpleSocket(address, sock)
{}

std::string ClientSocket::get_buffer() {
	return buffer;
}

bool		ClientSocket::read_to_buffer() {

	char	read_buffer[BUFFER_SIZE];
	int ret = read(get_sock(), read_buffer, BUFFER_SIZE);

	buffer += std::string(read_buffer, ret);
	return (true); //TODO only return true if request is complete
}
