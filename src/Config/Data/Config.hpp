#pragma once

#include <vector>
#include "ServerConfig.hpp"
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

        const std::vector<ServerConfig>& GetServers() const;
        const ServerConfig& GetDefaultServer() const;
    private:
        std::vector<ServerConfig> Servers;
        size_t DefaultServerIndex;
        // Class variables

        // Class functions

};
