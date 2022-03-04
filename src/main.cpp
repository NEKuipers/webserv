
#include "ConfigFile.hpp"
#include "Lexer.hpp"

#include "fstream"
#include <iostream>
#include "Server.hpp"

#include "UnexpectedTokenException.hpp"

int         main(int argc, char **argv) {
	
	
	try {
		ConfigFile File("TestFile");

		std::cout << "Read file: " << File << std::endl;
		const std::vector<ConfigLine>& Lines = File.GetConfigLines();
		
		for (size_t i = 0; i < Lines.size(); i++)
		{
			Server s(Lines[i]);

			std::cout << "Got server: " << s << std::endl;
		}
	} catch (UnexpectedTokenException E) {
		std::cout << "Unexpected token: " << E.Actual << ", Expected: " << E.Expected << "!" << std::endl;
	}



    (void)argc;
    (void)argv;
    return (0);
}
