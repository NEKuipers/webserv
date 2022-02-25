
#include <iostream>

int         main(int argc, char **argv) 
{
    std::string conf_filename;

    if (argc != 2) {
        std::cerr << "Invalid number of arguments." << std::endl;
        return (1);
    }
    conf_filename = argv[1];
    return (0);
}
