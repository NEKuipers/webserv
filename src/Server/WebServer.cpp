#include "WebServer.hpp"
// #include "ReadFromSocketException.hpp"
#include "SelectErrorException.hpp"
#include <ctime>
#include <iostream>
#include <fstream>

WebServer::WebServer(int domain, int service, int protocol, int port, u_long interface, int bklg)
{
	ServerSocket *newsocket = new ServerSocket(domain, service, protocol, port, interface, bklg);
	accept_sockets.push_back(newsocket);
	launch();
}

WebServer::WebServer()
{
	ServerSocket *newsocket = new ServerSocket(AF_INET, SOCK_STREAM,0,20480,INADDR_ANY,10);
	accept_sockets.push_back(newsocket);
	launch();
}

WebServer::WebServer(Config &config)
{
	std::vector<std::pair<in_addr_t, in_port_t> > *Vec = config.GetListenConnections();

	for (std::vector<std::pair<in_addr_t, in_port_t> >::iterator iter = Vec->begin(); iter != Vec->end(); iter++)
	{
		ServerSocket *newsocket = new ServerSocket(AF_INET, SOCK_STREAM, 0, iter->second, iter->first, 10);
		accept_sockets.push_back(newsocket);
	}
	launch();
}


WebServer::WebServer(const WebServer &src)
{
    *this = src;
}

WebServer		&WebServer::operator=(const WebServer &rhs)
{
	this->accept_sockets = rhs.accept_sockets;
	return (*this);
}

WebServer::~WebServer(){}

int	WebServer::connectionAccepter(ServerSocket *conn_socket)
{
	int client;
	struct sockaddr_in address = conn_socket->get_address();
	int addrlen = sizeof(address);
	if ((client = accept(conn_socket->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
		throw ConnectionErrorException();
	return (client);
}

bool		WebServer::connectionHandler(ClientSocket *conn_socket)
{
	enum read_status status = conn_socket->read_to_request();
	if (status == NOT_COMPLETE)
		return false;

	// IF: status == HEADER_COMPLETE, make config request & see what file it should return
	// IF: status == BODY_COMPLETE OR it should return something that requires the body, call read_to_request AGAIN
		
	std::cout << "======START OF REQUEST======="<<std::endl;
	Request new_request = conn_socket->get_request();
	std::cout << new_request << std::endl;
	std::cout << "======END OF REQUEST======"<<std::endl;
	//response
	// std::ifstream resp("resources/index.html");
	// std::string response((std::istreambuf_iterator<char>(resp)), std::istreambuf_iterator<char>());

	std::string response = "<!DOCTYPE html><head><title>Webserv Testpage</title></head><body><p>Hello World!\n It is ";
	time_t now = time(0);
	char *datetime = ctime(&now);
	response.append(datetime);
	response.append("<p></body></html>");
	//TODO: Add the creation of requested response here
	write(conn_socket->get_sock(), response.c_str(), response.size());
	close(conn_socket->get_sock());
	return (true);
}

static void	sigintHandler(int signum)
{
	std::cerr << "\b\bServer interrupted. Exiting." << std::endl;
	SimpleSocket::close_all();
	exit(signum);
}

int	WebServer::launch()
{
	fd_set save_read_fds = this->get_socket_fd_set();
 	fd_set save_write_fds;
	FD_ZERO(&save_write_fds);
	int max_fd = this->get_highest_fd();
	signal(SIGINT, sigintHandler);
	std::cout << "Server is running!"<<std::endl;
	while(true)
	{
		fd_set read_fds = save_read_fds;
		if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1)
			throw SelectErrorException();
		for (size_t count = 0; count < accept_sockets.size(); count++) 
		{
			if (FD_ISSET(accept_sockets[count]->get_sock(), &read_fds)) 
			{
				try {
					int new_client = connectionAccepter(accept_sockets[count]);
					FD_SET(new_client, &save_read_fds);
					max_fd = std::max(max_fd, new_client);
					read_sockets.push_back(new ClientSocket(accept_sockets[count]->get_address(), new_client));
				} catch (std::exception &e) {
					std::cout << e.what() << std::endl;
				}
			}
		}
		for (size_t count = 0; count < read_sockets.size(); count++) 
		{
			if (FD_ISSET(read_sockets[count]->get_sock(), &read_fds)) 
			{
				try {
					if (connectionHandler(read_sockets[count]))
					{
						FD_CLR(read_sockets[count]->get_sock(), &save_read_fds);
						read_sockets.erase(read_sockets.begin() + count--);
					}
				} catch (std::exception &e) {
					std::cout << e.what() << std::endl;
				}
			}
		}
	}
	return (0);
}

std::vector<ServerSocket *>		WebServer::get_sockets()
{
	return this->accept_sockets;
}

fd_set							WebServer::get_socket_fd_set()
{
	fd_set	socket_fd_set;

	FD_ZERO(&socket_fd_set);
	for (std::vector<ServerSocket *>::iterator it = this->accept_sockets.begin(); it != this->accept_sockets.end(); it++)
	{
		FD_SET((*it)->get_sock(), &socket_fd_set);
	}
	return (socket_fd_set);
}

int							WebServer::get_highest_fd()
{
	return ((*(this->accept_sockets.end() - 1))->get_sock());
}
