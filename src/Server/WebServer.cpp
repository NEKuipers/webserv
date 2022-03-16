#include "WebServer.hpp"

WebServer::WebServer(int domain, int service, int protocol, int port, u_long interface, int bklg)
{
	ServerSocket *newsocket = new ServerSocket(domain, service, protocol, port, interface, bklg);
	sockets.push_back(newsocket);
	launch();
}

WebServer::WebServer()
{
	ServerSocket *newsocket = new ServerSocket(AF_INET, SOCK_STREAM,0,80,INADDR_ANY,10);
	sockets.push_back(newsocket);
	launch();
}

//WebServer::WebServer(Configuration config) {}


WebServer::WebServer(const WebServer &src)
{
    *this = src;
}

WebServer		&WebServer::operator=(const WebServer &rhs)
{
	this->sockets = rhs.sockets;
	return (*this);
}

WebServer::~WebServer(){}

void	WebServer::connectionAccepter(ServerSocket *conn_socket)
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

void	WebServer::connectionHandler(ServerSocket *conn_socket)
{
	(void)conn_socket;
	//TODO: Add parsing and inputvalidation for the HTTP request here
	std::cout << buffer << std::endl;
}

void	WebServer::connectionResponder(ServerSocket *conn_socket)
{
	#include <ctime>
	time_t now = time(0);
	char *datetime = ctime(&now);
	std::string response = "Test\n";
	response.append(datetime);
	
	//TODO: Add the creation of requested response here
	write(conn_socket->get_sock_fd(), response.c_str(), response.size());
}

void	WebServer::connectionCloser(ServerSocket *conn_socket)
{
	close(conn_socket->get_sock_fd());
}

static void	sigintHandler(int signum)
{
	std::cerr << "\b\bServer interrupted. Exiting." << std::endl;
	exit(signum);
}

void	WebServer::launch()
{
	fd_set 	read_fds;
	fd_set 	write_fds;
	int		max_fd;

	FD_ZERO(&write_fds);
	read_fds = this->get_socket_fd_set();
	max_fd = this->get_highest_fd();
	signal(SIGINT, sigintHandler);
	std::cout << "Server is running!"<<std::endl;
	while(true)
	{
		select(max_fd + 1, &read_fds, &write_fds, NULL, NULL);
		for (size_t count = 0; count < sockets.size(); count++)
		{
			connectionAccepter(sockets[count]);
			connectionHandler(sockets[count]);
			connectionResponder(sockets[count]);
			connectionCloser(sockets[count]);
		}
	}
}

std::vector<ServerSocket *>		WebServer::get_sockets()
{
	return this->sockets;
}

fd_set							WebServer::get_socket_fd_set()
{
	fd_set	socket_fd_set;
	
	FD_ZERO(&socket_fd_set);
	for (std::vector<ServerSocket *>::iterator it = this->sockets.begin(); it != this->sockets.end(); it++)
	{
		FD_SET((*it)->get_sock_fd(), &socket_fd_set);
	}
	return (socket_fd_set);
}

int							WebServer::get_highest_fd()
{
	return ((*(this->sockets.end() - 1))->get_sock_fd());
}
