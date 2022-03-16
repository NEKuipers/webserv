
#include "ConfigFile.hpp"
#include "Lexer.hpp"

#include "fstream"
#include <iostream>
#include "ServerConfig.hpp"

#include "Config.hpp"
#include "UnexpectedTokenException.hpp"

int         main(int argc, char **argv) {


	try {
		Config Conf(ConfigFile("TestFile"));

		std::cout << Conf << std::endl;
	} catch (UnexpectedTokenException E) {
		std::cout << E << std::endl;
	}



    (void)argc;
    (void)argv;
    return (0);
}
