
#include "ConfigFile.hpp"
#include "ServerSocket.hpp"
#include "WebServer.hpp"
#include "Lexer.hpp"
#include "Config.hpp"
#include "FileResponse.hpp"

#include "fstream"
#include <iostream>
#include "UnexpectedTokenException.hpp"
#include "ConvertException.hpp"
#include "MethodException.hpp"
#include "ConnectionErrorException.hpp"
#include "Response.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "CGIRunner.hpp"

int         main(int argc, char **argv)
{
	// if (argc != 2) {
	// 	std::cerr << "Error: incorrect amount of arguments.\n \
	// 	Please run the program as follows: ./webserv <config_file>" << std::endl;
	// 	return (1);
	// }
	(void)argc;
	(void)argv;

	Response::InitContentTypes();
	FileResponse::InitContentTypes();

	try {
		Config Conf(ConfigFile(argc == 2 ? argv[1] : "./SmallConfig.conf"));


		std::cout << "Starting...." <<std::endl;
		WebServer testserver = WebServer(Conf);

	} catch (UnexpectedTokenException E) {
		std::cout << E << std::endl;
		return 1;
	} catch (ConvertException E) {
		std::cout << E << std::endl;
		return 1;
	} catch (MethodException E) {
		std::cout << E << std::endl;
		return 1;
	} catch (ConnectionErrorException E) {
		std::cout << E.what() << std::endl;
		return 1;
	}

    return (0);
}
