
#include "ConfigFile.hpp"
#include "ServerSocket.hpp"
#include "WebServer.hpp"
#include "Lexer.hpp"
#include "Config.hpp"

#include "fstream"
#include <iostream>
#include "UnexpectedTokenException.hpp"
#include "ConvertException.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static void PrintResponse(const Config& Conf, const ConfigRequest& Request)
{
	ConfigResponse* Response = Conf.GetResponse(Request);
	if (!Response)
	{
		std::cout << "No Response!" << std::endl;
		return;
	}
	
	Response->Print(std::cout);
	std::cout << std::endl;
	delete Response;	// Cleanup
}

int         main(int argc, char **argv) 
{
	// if (argc != 2) {
	// 	std::cerr << "Error: incorrect amount of arguments.\n \
	// 	Please run the program as follows: ./webserv <config_file>" << std::endl;
	// 	return (1);
	// }
	(void)argc;
	(void)argv;
	
	try {
		Config Conf(ConfigFile("SmallConfig.conf"));
		std::cout << Conf << std::endl;

		in_addr Addr;
		inet_aton("127.0.0.1", &Addr);

		// Note: Currently it does not use the URI, so the "File1" stuff is pretty pointless now
		PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(27162), "test.com", "File1", 0, "GET"));
		PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(82712), "test.com", "File2", 0, "GET"));
		PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(82712), "test.com", "File2", 128, "GET"));
		PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(82712), "test.com", "File3", 128, "GET"));
		PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(82712), "test.com", "File1", 128, "GET"));	// This will go to the error page
		PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(82712), "test.com", "File4", ~0, "GET"));	// Max content-length, too big for anything to accept it, will try to redirect to ErrorPage, but will be too big for even the error page... Not sure how im going to handle that...

		return 0;

		std::cout << "Starting...." <<std::endl;
		WebServer testserver = WebServer();

		//Response = Conf.GetResponse(Request);

	} catch (UnexpectedTokenException E) {
		std::cout << E << std::endl;
		return 1;
	} catch (ConvertException E) {
		std::cout << E << std::endl;
		return 1;
	}
	
    return (0);
}
