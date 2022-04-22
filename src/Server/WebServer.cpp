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
	struct sockaddr_in address;// = conn_socket->get_address();
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
				conn_socket->get_request().get_header_value("Host"),
				conn_socket->get_request().get_request_line().path,
				atoi(conn_socket->get_request().get_header_value("Content-Length").c_str()), //TODO make c++
				conn_socket->get_request().get_request_line().method
			));
		}

		if (!conn_socket->check_body())
			return false;
		// We have now read the whole packet, if we want to read the body, we have also read that, send back the stuff
	}

	Request new_request = conn_socket->get_request();
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


bool	WebServer::onAccept(std::pair<WebServer*, ServerSocket*>* Arg, int ClientFD, const struct sockaddr& Address, socklen_t AddressLen)
{
	WebServer* Server = Arg->first;
	ServerSocket* Socket = Arg->second;

	if (ClientFD == -1)
		return false;

	struct sockaddr_in addr;
	memcpy(&addr, &Address, AddressLen);	// What
	ClientSocket *NewSocket = new ClientSocket(Socket->get_address(), ClientFD, addr);

	Server->selector.OnRead(ClientFD, new std::pair<WebServer*, ClientSocket*>(Server, NewSocket), (Selector::OnReadFunction)onRead);

	return false;
}

static bool CloseInput(const CGIRunner *Runner, bool LastWrite, int StartByte, int NumBytes)
{
	if (LastWrite)
		close(Runner->InputFD);
	(void)StartByte;
	(void)NumBytes;
	
	return LastWrite;
}

bool	WebServer::onRead(std::pair<WebServer*, ClientSocket*>* Arg, bool LastRead, const std::string& Read)
{
	WebServer* Server = Arg->first;
	ClientSocket* Client = Arg->second;

	Client->read(Read);

	if (!Server->IsRequestComplete(Client))
	{
		if (LastRead)
		{
			std::cerr << "Somehow, the last message the client sent was not a complete request!" << std::endl;

			// Just send a Bad request response, i dunno
			Server->selector.Write(Client->get_sock(), Response::create_status_line(400) + "\r\n", Client, (Selector::OnWriteFunction)onWriteCloseAfterComplete);
			delete Arg;
			return true;
		}
		return false;
	}

	Client->createResponse();

	Response* Response = Client->get_http_response();

	if (CGIResponse* CgiResponsePtr = dynamic_cast<CGIResponse*>(Response))
	{
		// Write the body to the CGI program
		//std::cout << "Writing body: " << Client->get_request().get_body() << std::endl;
		Server->selector.Write(CgiResponsePtr->get_cgi_runner()->InputFD, Client->get_request().get_body(), (void*)CgiResponsePtr->get_cgi_runner(), (Selector::OnWriteFunction)CloseInput);

		// The CGI Programs output does not write most of the headers, so write it here
		const int status_code = 200;
		std::string BaseResponse = Response::create_status_line(status_code);
		BaseResponse += Response::create_headers(Client->conf_response, Client->get_request(), status_code);
		Server->selector.Write(Client->get_sock(), BaseResponse, NULL, NULL);

		// Read from the CGI program's output, The onCgiRead function will call write() on the client socket with whatever it read
		Server->selector.OnRead(CgiResponsePtr->get_cgi_runner()->OutputFD, Arg, (Selector::OnReadFunction)onCgiRead);

		// Do not delete arg, it is used as a new Arg in OnRead, and that will delete if after
		return true;
	}
	else if (SimpleResponse* SimpleResponsePtr = dynamic_cast<SimpleResponse*>(Response))
	{
		std::string Message = SimpleResponsePtr->get_response_string();

		Server->selector.Write(Client->get_sock(), Message, Client, (Selector::OnWriteFunction)onWriteCloseAfterComplete);

		// Last time we read from the client
		delete Arg;
		return true;
	}

	throw "Unreachable";
}
bool	WebServer::onCgiRead(std::pair<WebServer*, ClientSocket*>* Arg, bool LastRead, const std::string& Read)
{
	WebServer* Server = Arg->first;
	ClientSocket* Client = Arg->second;

	//std::cout << "Read from CGI: " << Read << std::endl;
	//if (LastRead)
	//	std::cout << "Last!" << std::endl;
	(void)LastRead;
	(void)Read;


	Server->selector.Write(Client->get_sock(), Read, Client, LastRead ? (Selector::OnWriteFunction)onWriteCloseAfterComplete : NULL);

	// TODO: read the "Content-Length" header if it exists, and then make sure we do not read more than that
	// Actually, what happens if it EOF's and we have read less than the content length?

	if (LastRead)
	{
		//std::cout << "Closing!" << std::endl;
		close(static_cast<CGIResponse*>(Client->get_http_response())->get_cgi_runner()->OutputFD);
		delete Arg;
	}
	return LastRead;
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
	 	selector.OnAccept((*it)->get_sock(), new std::pair<WebServer*, ServerSocket*>(this, *it), (Selector::OnAcceptFunction)onAccept);	// NOTE: These pairs are leaked when selector ends, only possible if select() returns -1

	return selector.Start();
}

std::vector<ServerSocket *>		WebServer::get_sockets()
{
	return this->server_sockets;
}
