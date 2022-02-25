
#include "ConfigFile.hpp"
#include "Parser.hpp"

#include "fstream"
#include <iostream>

int         main(int argc, char **argv) {
	/*
	std::fstream Stream("ConfigExample");
	Parser p(Stream);

	while (p.Peek() != EndOfFile)
		std::cout << "type: " << p.Peek() << " read: " << p.Read(p.Peek()) << std::endl;
	*/

	ConfigFile File("ConfigExample");

	std::cout << File << std::endl;

    if (argc != 2) {
        std::cout << "ERROR: no argument" << std::endl;
        return (1);
    }
    
    return (0);
}
