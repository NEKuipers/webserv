
#include "ConfigFile.hpp"
#include "ServerSocket.hpp"
#include "WebServer.hpp"
#include "Lexer.hpp"
#include "Config.hpp"

#include "fstream"
#include <iostream>
#include "UnexpectedTokenException.hpp"
#include "ConvertException.hpp"
#include "MethodException.hpp"
#include "ConnectionErrorException.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// static void PrintResponse(const Config& Conf, const ConfigRequest& Request)
// {
// 	ConfigResponse* Response = Conf.GetResponse(Request);

// 	std::cout << Request << " => ";
// 	if (!Response)
// 	{
// 		std::cout << "No Response!" << std::endl;
// 		return;
// 	}

// 	Response->Print(std::cout);
// 	std::cout << std::endl;
// 	delete Response;	// Cleanup
// }

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
		Config Conf(ConfigFile("./SmallConfig.conf"));
		// std::cout << Conf << std::endl;

		in_addr Addr;
		inet_aton("127.0.0.1", &Addr);

		std::vector<std::pair<in_addr_t, in_port_t> >* Listens = Conf.GetListenConnections();

		for (std::vector<std::pair<in_addr_t, in_port_t> >::const_iterator It = Listens->begin(); It != Listens->end(); It++)
		{
			in_addr DumbHistoricalReasonsStruct;
			DumbHistoricalReasonsStruct.s_addr = It->first;
			std::cout << "Listening on: " << inet_ntoa(DumbHistoricalReasonsStruct) << ":" << ntohs(It->second) << std::endl;
		}
		delete Listens;

		// Note: Currently it does not use the URI, so the "File1" stuff is pretty pointless now
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(27162), "test1.com", "File1", 0, "GET"));
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(17176), "test1.com", "File2", 0, "GET"));
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(17176), "test1.com", "File2", 128, "GET"));
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(17176), "test1.com", "File3", 128, "GET"));
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(17176), "test1.com", "File1", 128, "GET"));	// This will go to the error page
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(17176), "test1.com", "File4", ~0, "GET"));	// Max content-length, too big for anything to accept it, will try to redirect to ErrorPage, but will be too big for even the error page... Not sure how im going to handle that...

		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(27162), "test2.com", "File1", 0, "GET"));
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(17176), "test2.com", "File2", 0, "GET"));
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(17176), "test2.com", "test/File2", 0, "GET"));
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(17176), "test2.com", "test/File1", 0, "GET"));
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(17176), "test2.com", "forbidden/secrets", 0, "GET"));
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(80), "unknown.com", "File1", 0, "GET"));	// This one should return default80/File1
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(17176), "unknown.com", "File1", 0, "GET"));	// And this one default/File1
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(17176), "test1.com", "../../Makefile", 0, "GET"));	// Yeah, no
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(17176), "test2.com", "test/../forbidden/secrets", 0, "GET"));	// Also no, even though the root has not changed

		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(80), "test2.com", "post/File1", 128, "POST"));	// Well...

		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(17176), "unknown.com", "add.cgi", 0, "GET"));

		// Mhhh, yeah, still some problems here, not sure how i can prevent this though
		// PrintResponse(Conf, ConfigRequest(Addr.s_addr, htons(17176), "test2.com", "test/../File2", 0, "GET"));	// Gets supposedly overridden File2


		// return 0;

		std::cout << "Starting...." <<std::endl;
		WebServer testserver = WebServer(Conf);

		//Response = Conf.GetResponse(Request);

	} catch (UnexpectedTokenException E) {
		std::cout << E << std::endl;
		return 1;
	} catch (ConvertException E) {
		std::cout << E << std::endl;
		return 1;
	} catch (MethodException E) {
		std::cout << E << std::endl;
		return 1;
	}

    return (0);
}
