
#include "ConfigFile.hpp"
#include "ServerSocket.hpp"
#include "WebServer.hpp"
#include "Lexer.hpp"

#include "fstream"
#include <iostream>
#include "ServerConfig.hpp"

#include "Config.hpp"
#include "UnexpectedTokenException.hpp"

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
		Config Conf(ConfigFile("TestFile"));

		std::cout << Conf << std::endl;

		std::cout << "Starting...." <<std::endl;
		WebServer testserver = WebServer();

	} catch (UnexpectedTokenException E) {
		std::cout << E << std::endl;
	}
	
    return (0);
}
