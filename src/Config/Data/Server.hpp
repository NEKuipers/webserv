#pragma once

#include <vector>
#include "Location.hpp"
#include "ConfigLine.hpp"
#include <netinet/in.h>

class Server {
    public:
        static const std::string DESCRIPTOR;
        static const in_port_t DEFAULT_PORT;

        Server();
        Server(const ConfigLine& Line);
        Server(const Server& From);

        ~Server();

        Server& operator = (const Server& From);

        // Public functions
        const Location& GetLocation(const std::string& Search) const;
        bool RepliesTo(const std::string& ServerName) const;
        bool GetIsDefaultServer() const;
        
        friend std::ostream& operator<<(std::ostream& Stream, const Server& Server);
    private:
        // Class variables
        std::vector<Location> Locations;

        bool IsDefaultServer;   // In case no server names match, is this the default server?
        std::vector<std::string> ServerNames;

        std::vector<std::pair<in_addr, in_port_t> > ListenLocations;

        // Class functions
        
};
