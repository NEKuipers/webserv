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

void	Server::connectionAccepter(ServerSocket *conn_socket)
{
	int newsock;
	struct sockaddr_in address = conn_socket->get_address();
	int addrlen = sizeof(address);
	try 
	{
		if ((newsock = accept(conn_socket->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
			throw ConnectionErrorException();
	}
	catch (std::exception &e)
	{
		std::cerr<<e.what()<<std::endl;
	}
	conn_socket->set_sock_fd(newsock);
	read(conn_socket->get_sock_fd(), buffer, 30000);
}

void	Server::connectionHandler(ServerSocket *conn_socket)
{
	(void)conn_socket;
	//TODO: Add parsing and inputvalidation for the HTTP request here
	std::cout << buffer << std::endl;
}

void	Server::connectionResponder(ServerSocket *conn_socket)
{
	#include <ctime>
	time_t now = time(0);
	char *datetime = ctime(&now);
	std::string response = "Test\n";
	response.append(datetime);
	
	//TODO: Add the creation of requested response here
	write(conn_socket->get_sock_fd(), response.c_str(), response.size());
}

void	Server::connectionCloser(ServerSocket *conn_socket)
{
	close(conn_socket->get_sock_fd());
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
		connectionAccepter(socket);
		connectionHandler(socket);
		connectionResponder(socket);
		connectionCloser(socket);
		std::cout << "===== DONE =====" << std::endl;
	}
}

ServerSocket	*Server::get_socket()
{
	return socket;
}
