
#include "ConfigFile.hpp"
#include "Parser.hpp"
#include "ListeningSocket.hpp"
#include "TestServer.hpp"

#include "fstream"
#include <iostream>

int         main(int argc, char **argv) {
	/*
	std::fstream Stream("ConfigExample");
	Parser p(Stream);

	while (p.Peek() != EndOfFile)
		std::cout << "type: " << p.Peek() << " read: " << p.Read(p.Peek()) << std::endl;
	*/

	// ConfigFile File("ConfigExample");

	// std::cout << File << std::endl;

    // if (argc != 2) {
    //     std::cout << "ERROR: no argument" << std::endl;
    //     return (1);
    // }
	(void)argc;
	(void)argv;
	
	std::cout << "Starting..." <<std::endl;
	// std::cout << "Binding Socket..." <<std::endl;
	// BindingSocket bs = BindingSocket(AF_INET, SOCK_STREAM, 0 , 81, INADDR_ANY);
	// std::cout << "Listening Socket..."<<std::endl;
	// ListeningSocket ls = ListeningSocket(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10);
	// std::cout << "Success!"<<std::endl;
	TestServer ts = TestServer();

    return (0);
}
