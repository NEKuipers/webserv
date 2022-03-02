
#include "ConfigFile.hpp"
#include "Lexer.hpp"

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

    (void)argc;
    (void)argv;
    return (0);
}
