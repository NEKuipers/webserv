#include "WebServer.hpp"
// #include "ReadFromSocketException.hpp"
#include "SelectErrorException.hpp"
#include <ctime>
#include <iostream>
#include <fstream>
#include "ToString.hpp"
#include "CGIRunner.hpp"
#include "ConfigFileResponse.hpp"
#include "SimpleResponse.hpp"
#include "CGIResponse.hpp"
#include <cassert>	// linux assert()

WebServer::WebServer(int domain, int service, int protocol, int port, u_long interface, int bklg) : selector(), configuration(NULL)
{
	ServerSocket *newsocket = new ServerSocket(domain, service, protocol, port, interface, bklg);
	server_sockets.push_back(newsocket);
	launch();
}

WebServer::WebServer() : selector(), configuration(NULL)
{
	ServerSocket *newsocket = new ServerSocket(AF_INET, SOCK_STREAM,0,20480,INADDR_ANY,10);
	server_sockets.push_back(newsocket);
	launch();
}

WebServer::WebServer(Config &config) : selector(), configuration(&config)
{
	std::vector<std::pair<in_addr_t, in_port_t> > *Vec = config.GetListenConnections();

	for (std::vector<std::pair<in_addr_t, in_port_t> >::iterator iter = Vec->begin(); iter != Vec->end(); iter++)
	{
		ServerSocket *newsocket = new ServerSocket(AF_INET, SOCK_STREAM, 0, iter->second, iter->first, 10);
		server_sockets.push_back(newsocket);
	}

	launch();
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

bool		WebServer::IsRequestComplete(ClientSocket *conn_socket)
{
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


bool	WebServer::onAccept(std::pair<WebServer*, ServerSocket*>* Arg, int ClientFD, struct sockaddr Address, socklen_t AddressLen)
{
	if (ClientFD == -1)
		return false;

	struct sockaddr_in addr;
	memcpy(&addr, &Address, AddressLen);	// What
	ClientSocket *socket = new ClientSocket(Arg->second->get_address(), ClientFD, addr);

	Arg->first->selector.OnRead(ClientFD, new std::pair<WebServer*, ClientSocket*>(Arg->first, socket), (Selector::OnReadFunction)onRead);

	return false;
}
bool	WebServer::onRead(std::pair<WebServer*, ClientSocket*>* Arg, bool LastRead, const std::string& Read)
{
	Arg->second->read(Read);

	if (!Arg->first->IsRequestComplete(Arg->second))
	{
		if (LastRead)
			delete Arg;
		return false;
	}

	Arg->second->createResponse();


	Response* Response = Arg->second->get_http_response();

	if (CGIResponse* CgiResponsePtr = dynamic_cast<CGIResponse*>(Response))
	{
		// TODO: Have some sort of OnCgiRead & OnCgiWrite function, Call OnRead(Cgi->OutputFD, Arg->second, OnCgiRead)
		// OnCgiRead will call Selector.Write() with what it read, if it was the last bit that it should read, pass onWriteCloseAfterComplete as the function
		// Also call Selector.Write() with the body, that will do the entire writing thing too!

		Arg->first->selector.Write(CgiResponsePtr->get_cgi_runner()->InputFD, Arg->second->get_request().get_body(), NULL, (Selector::OnWriteFunction)onWriteCloseAfterComplete);

		(void)CgiResponsePtr;
		Arg->first->selector.Write(Arg->second->get_sock(), "HTTP/1.1 200 OK\r\n\r\ncgi thing\r\n", Arg->second, (Selector::OnWriteFunction)onWriteCloseAfterComplete);
		//Arg->first->selector.OnRead(CgiResponsePtr->get_cgi_runner()->OutputFD, Arg, (Selector::OnWriteFunction)onCgiRead);
		return true;
	}
	else
	{
		std::string Message;
		bool Complete = Response->get_response_string(Message);
		assert(Complete);

		Arg->first->selector.Write(Arg->second->get_sock(), Message, Arg->second, (Selector::OnWriteFunction)onWriteCloseAfterComplete);
	}

	// Last time we read from the client
	delete Arg;
	return true;
}
bool	WebServer::onCgiRead(std::pair<WebServer*, ClientSocket*>* Arg, bool LastRead, const std::string& Read)
{
	(void)LastRead;
	(void)Read;
	Arg->first->selector.Write(Arg->second->get_sock(), "HTTP/1.1 200 OK\r\n\r\ncgi thing\r\n", Arg->second, (Selector::OnWriteFunction)onWriteCloseAfterComplete);
	
	return false;
}
bool	WebServer::onWriteCloseAfterComplete(ClientSocket* Arg, bool LastWrite, int StartByte, int NumBytes)
{
	(void)StartByte;
	(void)NumBytes;

	if (LastWrite)
		delete Arg;
	return false;
}

int	WebServer::launch()
{
	signal(SIGINT, sigintHandler);

	for (std::vector<ServerSocket*>::iterator it = server_sockets.begin(); it != server_sockets.end(); it++)
	 	selector.OnAccept((*it)->get_sock(), new std::pair<WebServer*, ServerSocket*>(this, *it), (Selector::OnAcceptFunction)onAccept);	// NOTE: These pairs are leaked when selector ends

	return selector.Start();
}

std::vector<ServerSocket *>		WebServer::get_sockets()
{
	return this->server_sockets;
}
