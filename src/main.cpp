
#include "ConfigFile.hpp"
#include "ServerSocket.hpp"
#include "WebServer.hpp"
#include "Lexer.hpp"
#include "Config.hpp"

#include "fstream"
#include <iostream>
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
		Config Conf(ConfigFile("Settings.conf"));

		//std::cout << Conf << std::endl;

		std::cout << "Starting...." <<std::endl;
		WebServer testserver = WebServer();

		//Response = Conf.GetResponse(Request);

	} catch (UnexpectedTokenException E) {
		std::cout << E << std::endl;
	}
	WebServer testserver = WebServer();
	
    return (0);
}
