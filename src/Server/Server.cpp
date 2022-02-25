#include "Server.hpp"

Server::Server(int domain, int service, int protocol, int port, u_long interface, int bklg)
{
	socket = new ListeningSocket(domain, service, protocol, port, interface, bklg);
	launch();
}

Server::Server()
{
	socket = new ListeningSocket(AF_INET, SOCK_STREAM,0,80,INADDR_ANY,10);
	launch();
}

Server::~Server()
{
	//delete socket;
}

void	Server::connectionAccepter()
{
	struct sockaddr_in address = get_socket()->get_address();
	int addrlen = sizeof(address);
	if ((new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
	{
		throw ("Error in accepting connection");
		exit(EXIT_FAILURE);
	}
	read(new_socket, buffer, 30000);
}

void	Server::connectionHandler()
{
	//TODO: Add parsing and inputvalidation for the HTTP request here
	std::cout << buffer << std::endl;
}

void	Server::connectionResponder()
{
	std::string response = "Webserv says hi\n";
	
	//TODO: Add the creation of requested response here
	write(new_socket, response.c_str(), response.size());
	close(new_socket);
}


static void	sigintHandler(int signum)
{
	std::cerr << "\b\bServer interrupted. Exiting." << std::endl;
	exit(signum);
}

void	Server::launch()
{
	signal(SIGINT, sigintHandler);
	while(true)
	{
		std::cout << "===== WAITING =====" << std::endl;
		connectionAccepter();
		connectionHandler();
		connectionResponder();
		std::cout << "===== DONE =====" << std::endl;
	}
}

ListeningSocket	*Server::get_socket()
{
	return socket;
}
