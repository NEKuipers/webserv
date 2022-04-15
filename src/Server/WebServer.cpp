#include "WebServer.hpp"
// #include "ReadFromSocketException.hpp"
#include "SelectErrorException.hpp"
#include <ctime>
#include <iostream>
#include <fstream>
#include "ToString.hpp"
#include "CGIRunner.hpp"
#include "FileResponse.hpp"

WebServer::WebServer(int domain, int service, int protocol, int port, u_long interface, int bklg) : configuration(NULL)
{
	ServerSocket *newsocket = new ServerSocket(domain, service, protocol, port, interface, bklg);
	accept_sockets.push_back(newsocket);
	launch();
}

WebServer::WebServer() : configuration(NULL)
{
	ServerSocket *newsocket = new ServerSocket(AF_INET, SOCK_STREAM,0,20480,INADDR_ANY,10);
	accept_sockets.push_back(newsocket);
	launch();
}

WebServer::WebServer(Config &config) : configuration(&config)
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

ClientSocket*	WebServer::connectionAccepter(ServerSocket *conn_socket)
{
	int client;
	struct sockaddr_in address = conn_socket->get_address();
	int addrlen = sizeof(address);
	if ((client = accept(conn_socket->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
		throw ConnectionErrorException();
	ClientSocket *socket = new ClientSocket(conn_socket->get_address(), client, address);
	return (socket);
}



bool		WebServer::connectionHandler(ClientSocket *conn_socket)
{
	conn_socket->read();

	bool status = conn_socket->check_headers();
	if (status == false)
		return false;

	// Do config stuff
	if (configuration)
	{
		if (status == true)
		{
			conn_socket->conf_response = configuration->GetResponse(ConfigRequest(
				conn_socket->get_address().sin_addr.s_addr,
				conn_socket->get_address().sin_port,
				"Unknown",	// TODO: Probabily in a header field
				conn_socket->get_request().get_request_line().target,
				0,	// TODO: Get Content-length
				conn_socket->get_request().get_request_line().method
			));
		}
		if (conn_socket->conf_response && conn_socket->conf_response->RequiresBody() && !conn_socket->check_body())
			return false;
		// We have now read the whole packet, if we want to read the body, we have also read that, send back the stuff
	}

	// std::cout << "======START OF REQUEST======="<<std::endl;
	Request new_request = conn_socket->get_request();
	// std::cout << new_request << std::endl;
	// std::cout << "======END OF REQUEST======"<<std::endl;

	return (true);
}

bool	WebServer::connectionResponder(ClientSocket *conn_socket)
{
	return (conn_socket->send());
}


static void	sigintHandler(int signum)
{
	std::cerr << "\b\bServer interrupted. Exiting." << std::endl;
	SimpleSocket::close_all();
	exit(signum);
}

template <typename T>
static void	clear_socket(std::vector<T*> &sockets, fd_set &save_read_fds, fd_set &save_write_fds, size_t &count)
{
	FD_CLR(sockets[count]->get_sock(), &save_read_fds);
	FD_CLR(sockets[count]->get_sock(), &save_write_fds);
	delete sockets[count];
	sockets.erase(sockets.begin() + count--);
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
		fd_set write_fds = save_write_fds;
		if (select(max_fd + 1, &read_fds, &write_fds, NULL, NULL) == -1)
			throw SelectErrorException();
		for (size_t count = 0; count < accept_sockets.size(); count++) 
		{
			if (FD_ISSET(accept_sockets[count]->get_sock(), &read_fds)) 
			{
				try {
					ClientSocket *new_socket = connectionAccepter(accept_sockets[count]);
					FD_SET(new_socket->get_sock(), &save_read_fds);
					max_fd = std::max(max_fd, new_socket->get_sock());
					read_sockets.push_back(new_socket);
				} catch (std::exception &e) {
					std::cout << e.what() << std::endl;
					clear_socket(read_sockets, save_read_fds, save_write_fds, count);
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
						read_sockets[count]->createResponse();
						FD_SET(read_sockets[count]->get_sock(), &save_write_fds);
					}
				} catch (std::exception &e) {
					std::cout << e.what() << std::endl;
					clear_socket(read_sockets, save_read_fds, save_write_fds, count);
				}
			}
			if (FD_ISSET(read_sockets[count]->get_sock(), &write_fds))
			{
				try {
					if (connectionResponder(read_sockets[count]))
						clear_socket(read_sockets, save_read_fds, save_write_fds, count);
				} catch (std::exception &e) {
					std::cout << e.what() << std::endl;
					clear_socket(read_sockets, save_read_fds, save_write_fds, count);
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
