#pragma once

#include <vector>
#include "Server.hpp"
#include "ConfigFile.hpp"

class Config {
    public: 
        Config();
        Config(const ConfigFile& File);
        Config(const Config& From);

        ~Config();

        Config& operator = (const Config& From);

        // Public functions
        friend std::ostream& operator<<(std::ostream& Stream, const Config& Config);
    private:
        std::vector<Server> Servers;
        // Class variables
        
        // Class functions
        
};
