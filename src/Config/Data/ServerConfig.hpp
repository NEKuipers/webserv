#pragma once

#include <vector>
#include "LocationConfig.hpp"
#include "ConfigLine.hpp"
#include <netinet/in.h>

class ServerConfig {
    public:
        static const std::string DESCRIPTOR;
        static const in_port_t DEFAULT_PORT;

        ServerConfig();
        ServerConfig(const ConfigLine& Line);
        ServerConfig(const ServerConfig& From);

        ~ServerConfig();

        ServerConfig& operator = (const ServerConfig& From);

        // Public functions
        const LocationConfig& GetLocation(const std::string& Search) const;
        bool RepliesTo(const std::string& ServerName) const;
        bool GetIsDefaultServer() const;

        friend std::ostream& operator<<(std::ostream& Stream, const ServerConfig& Server);
    private:
        // Class variables
        std::vector<LocationConfig> Locations;

        bool IsDefaultServer;   // In case no server names match, is this the default server?
        std::vector<std::string> ServerNames;

        std::vector<std::pair<in_addr, in_port_t> > ListenLocations;

        // Class functions

};
