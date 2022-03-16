
#include "ConfigFile.hpp"
#include "Parser.hpp"
#include "ServerSocket.hpp"
#include "WebServer.hpp"
#include "Lexer.hpp"

#include "fstream"
#include <iostream>
#include "Server.hpp"

#include "Config.hpp"
#include "UnexpectedTokenException.hpp"

int         main(int argc, char **argv) {
	
	// std::fstream Stream("ConfigExample");
	// Parser p(Stream);

	// while (p.Peek() != EndOfFile)
	// 	std::cout << "type: " << p.Peek() << " read: " << p.Read(p.Peek()) << std::endl;
	
	// ConfigFile File("ConfigExample");

	// std::cout << File << std::endl;

    // if (argc != 2) {
    //     std::cout << "ERROR: no argument" << std::endl;
    //     return (1);
    // }
	(void)argc;
	(void)argv;
	std::cout << "Starting...." <<std::endl;
	WebServer testserver = WebServer();
	
	try {
		Config Conf(ConfigFile("TestFile"));

		std::cout << Conf << std::endl;
	} catch (UnexpectedTokenException E) {
		std::cout << E << std::endl;
	}



    return (0);
}
