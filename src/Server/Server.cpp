#include "Server.hpp"

Server::Server(int domain, int service, int protocol, int port, u_long interface, int bklg)
{
	socket = new ServerSocket(domain, service, protocol, port, interface, bklg);
	launch();
}

Server::Server()
{
	socket = new ServerSocket(AF_INET, SOCK_STREAM,0,80,INADDR_ANY,10);
	launch();
}

//Server::Server(Configuration config) {}


Server::Server(const Server &src)
{
    *this = src;
}

Server		&Server::operator=(const Server &rhs)
{
	this->sockets = rhs.sockets;
	return (*this);
}

Server::~Server(){}

void	Server::connectionAccepter()
{
	struct sockaddr_in address = get_socket()->get_address();
	int addrlen = sizeof(address);
	try 
	{
		if ((sock_fd = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
			throw ConnectionErrorException();
	}
	catch (std::exception &e)
	{
		std::cerr<<e.what()<<std::endl;
	}
	read(sock_fd, buffer, 30000);
}

void	Server::connectionHandler()
{
	//TODO: Add parsing and inputvalidation for the HTTP request here
	std::cout << buffer << std::endl;
}

void	Server::connectionResponder()
{
	std::string response = "Hoi browser\n";
	
	//TODO: Add the creation of requested response here
	write(sock_fd, response.c_str(), response.size());
}

void	Server::connectionCloser()
{
	close(sock_fd);
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
		connectionCloser();
		std::cout << "===== DONE =====" << std::endl;
	}
}

ServerSocket	*Server::get_socket()
{
	return socket;
}
