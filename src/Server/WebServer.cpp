#include "WebServer.hpp"
// #include "ReadFromSocketException.hpp"
#include "SelectErrorException.hpp"
#include <ctime>
#include <iostream>
#include <fstream>
#include "ToString.hpp"
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

int	WebServer::connectionAccepter(ServerSocket *conn_socket)
{
	int client;
	struct sockaddr_in address = conn_socket->get_address();
	int addrlen = sizeof(address);
	if ((client = accept(conn_socket->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
		throw ConnectionErrorException();
	return (client);
}

static void writeResponse(ClientSocket *conn_socket)
{
	std::string Status;
	// TODO: Response headers
	std::string Headers = "";
	std::string ContentType = "";
	std::string Body;


	if (FileResponse* FileResponsePtr = dynamic_cast<FileResponse*>(conn_socket->response))
	{
		Status = "200 OK";
		ContentType = FileResponsePtr->GetContentType();
		Body = to_string(FileResponsePtr->GetStream().rdbuf());
	}
	else
	{
		if (conn_socket->response)
		{
			Status = "200 OK";
			ContentType = "text/html";

			Body = "<!DOCTYPE html><head><title>Webserv Testpage</title></head><body><p>Hello World!\n";
			Body.append("Unknown response type: " + to_string(*conn_socket->response));
			Body.append("<p></body></html>");
		}
		else
		{
			// I guess this is a default error page or something i dunno
			Status = "404 OK";
			ContentType = "text/html";
			Body = "<!DOCTYPE html><head><title>Webserv ErrorPage</title></head><body><p>You have made a invalid request!<p></body></html>";
		}
	}

	// Debugging infos
	time_t now = time(0);
	char *datetime = ctime(&now);
	Body.append(datetime);

	if (conn_socket->response)
		Body.append("Response type: " + to_string(*conn_socket->response));

	std::string response = "HTTP/1.1 " + Status + "\r\n";
	response += Headers;
	response += "[Content-Type]: " + ContentType + "\r\n";
	response += "[Content-Length]: " + to_string(Body.length()) + "\r\n";
	response += "\r\n";	// End of headers
	response += Body;

	

	send(conn_socket->get_sock(), response.c_str(), response.size(), 0);
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
			conn_socket->response = configuration->GetResponse(ConfigRequest(
				conn_socket->get_address().sin_addr.s_addr,
				conn_socket->get_address().sin_port,
				"Unknown",	// TODO: Probabily in a header field
				conn_socket->get_request().get_request_line().target,
				0,	// TODO: Get Content-length
				conn_socket->get_request().get_request_line().method
			));
		}
		if (conn_socket->response && conn_socket->response->RequiresBody() && !conn_socket->check_body())
			return false;
		// We have now read the whole packet, if we want to read the body, we have also read that, send back the stuff

		
	}

	std::cout << "======START OF REQUEST======="<<std::endl;
	Request new_request = conn_socket->get_request();
	std::cout << new_request << std::endl;
	std::cout << "======END OF REQUEST======"<<std::endl;

	writeResponse(conn_socket);
	
	close(conn_socket->get_sock());	// NOTE: It is possibly easier to add the close function to the simplesocket destructor, thay way its automatic and cannot be forgotten
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
		fd_set write_fds = save_write_fds;
		if (select(max_fd + 1, &read_fds, &write_fds, NULL, NULL) == -1)
			throw SelectErrorException();
		std::cout << "Select complete!" << std::endl;
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
						delete read_sockets[count];
						read_sockets.erase(read_sockets.begin() + count--);
					}
				} catch (std::exception &e) {
					std::cout << e.what() << std::endl;
				}
			}
			// if (FD_ISSET(read_sockets[count]->get_sock(), &write_fds))//TODO Write fds implementation!
			// {
			// 	;
			// 	FD_CLR(read_sockets[count]->get_sock(), &save_write_fds);
			// }
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
