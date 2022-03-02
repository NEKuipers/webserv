#pragma once

#include <vector>
#include "Location.hpp"
#include "ConfigLine.hpp"

class Server {
    public:
        static const std::string DESCRIPTOR;

        Server();
        Server(const ConfigLine& Line);
        Server(const Server& From);

        ~Server();

        Server& operator = (const Server& From);

        // Public functions
        const Location& GetLocation(const std::string& Search);
        bool RepliesTo(const std::string& ServerName);
        bool GetIsDefaultServer();
        
    private:
        // Class variables
        std::vector<Location> Locations;

        bool IsDefaultServer;   // In case no server names match, is this the default server?
        std::vector<std::string> ServerNames;

        // TODO: Listen [ip]:[port]

        // TODO (Maybe): Default values for Location


        // Class functions
        
};
