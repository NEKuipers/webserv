#include "Config.hpp"
#include "ConvertException.hpp"
#include "PrefixStreambuf.hpp"

Config::Config() {}
Config::Config(const ConfigFile& File)
{
    const std::vector<ConfigLine>& Lines = File.GetConfigLines();
    
    for (size_t i = 0; i < Lines.size(); i++)
        Servers.push_back(Server(Lines[i]));

    // Some error checking
    bool HasDefaultServer = false;
    for (size_t i = 0; i < Servers.size(); i++)
        if (Servers[i].GetIsDefaultServer())
        {
            if (HasDefaultServer)
                throw ConvertException("Converting from File to Config failed! Multiple servers are default!");
            else
                HasDefaultServer = true;
        }
}

Config::Config(const Config& From)
{
    this->operator=(From);
}

Config::~Config()
{
    
}

Config& Config::operator = (const Config& From)
{
    this->Servers = From.Servers;

    // return the existing object so we can chain this operator
    return *this;
}

std::ostream& operator<<(std::ostream& Stream, const Config& Config)
{
    for (size_t i = 0; i < Config.Servers.size(); i++)
    {
        Stream << "Server " << i << ":" << std::endl;
        
        PrefixStreambuf Prefix(Stream, "  ");
        Stream << Config.Servers[i] << std::endl;
        Prefix.End();
    }
    return Stream;
}
