#include "TestServer.hpp"

TestServer::TestServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
	launch();
}

void	TestServer::connectionAccepter()
{
	struct sockaddr_in address = get_socket()->get_address();
	int addrlen = sizeof(address);
	if ((new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
	{
		perror("Error in accepting connection");
		exit(EXIT_FAILURE);
	}
	read(new_socket, buffer, 30000);
}

void	TestServer::connectionHandler()
{
	//TODO: Add parsing and inputvalidation for the HTTP request here
	std::cout << buffer << std::endl;
}

void	TestServer::connectionResponder()
{
	char *response;
	
	//TODO: Add the creation of requested response here
	response = strdup("Webserv is working");
	write(new_socket, response, strlen(response));
	free(response);
	close(new_socket);
}


static void	sigintHandler(int signum)
{
	std::cerr << "\b\bServer interrupted. Exiting." << std::endl;
	exit(signum);
}

void	TestServer::launch()
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
