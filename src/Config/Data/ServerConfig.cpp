#include "ServerConfig.hpp"
#include "ConvertException.hpp"
#include "LocationException.hpp"

#include "PrefixStreambuf.hpp"

// TODO: Not sure if all these are needed
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>	// Apparently to use std::strtoul i need to include this?

const std::string ServerConfig::DESCRIPTOR = "server";
const in_port_t ServerConfig::DEFAULT_PORT = htons(80);

ServerConfig::ServerConfig() {}
ServerConfig::ServerConfig(const ServerConfig& From)
{
    this->operator=(From);
}

ServerConfig::ServerConfig(const ConfigLine& Line) : Locations(), IsDefaultServer(false), ServerNames()
{
    const std::vector<std::string>& Args = Line.GetArguments();

    if (Args.size() != 1)
        throw ConvertException("Converting from Line to ServerConfig failed! Args.size() != 1");
    if (Args[0] != DESCRIPTOR)
        throw ConvertException("Converting from Line to ServerConfig failed! Args[0] != DESCRIPTOR");

    ConfigBlock* Block = Line.GetBlock();
    if (Block == NULL)
        throw ConvertException("Converting from Line to ServerConfig failed! Must contain a block!");

    const std::vector<ConfigLine>& Lines = Block->GetLines();
    for (size_t LineID = 0; LineID < Lines.size(); LineID++)
    {
        // TODO: More types

        const std::vector<std::string>& LineArgs = Lines[LineID].GetArguments();
        if (LineArgs.size() <= 0)
            throw ConvertException("Converting from Line to ServerConfig failed! Block contained empty line? Should be impossible!");


        // TODO: This if/else if/else if/else thing is going to be huge!
        //  Find a neat way to do it properly
        if (LineArgs[0] == LocationConfig::DESCRIPTOR)
            Locations.push_back(LocationConfig(Lines[LineID]));
        else if (LineArgs[0] == "server_name")
            ServerNames.insert(ServerNames.end(), LineArgs.begin() + 1, LineArgs.end());
        else if (LineArgs[0] == "listen")
        {
            for (size_t i = 1; i < LineArgs.size(); i++)
            {
                std::string AddrStr = LineArgs[i];
                in_port_t Port = DEFAULT_PORT;

                size_t split = AddrStr.find(":");
                if (split != std::string::npos)
                {
                    char* End;
                    unsigned long PortUL = std::strtoul(&AddrStr.c_str()[split + 1], &End, 10);
                    Port = (in_port_t)PortUL;

                    // Do error checking
                    if (Port != PortUL) // Out of bounds checking
                        throw ConvertException("Converting from Line to ServerConfig failed! Port too big!");
                    // Check if the string contained more than just a number
                    if (End != &AddrStr.c_str()[AddrStr.length()])
                        throw ConvertException("Converting from Line to ServerConfig failed! Port contained more than just a number!");

                    AddrStr = AddrStr.substr(0, split);
                    Port = htons(Port);
                }

                in_addr Addr;
                if (inet_aton(AddrStr.c_str(), &Addr) == 0)
                    throw ConvertException("Converting from Line to ServerConfig failed! Malformed location Adress!");

                ListenLocations.push_back(std::make_pair(Addr, Port));
            }
        }
        else if (LineArgs[0] == "default_server")
        {
            if (LineArgs.size() != 1)
                 throw ConvertException("Converting from Line to ServerConfig failed! default_server should not have arguments!");
            IsDefaultServer = true;
        }
        else
            throw ConvertException("Converting from Line to ServerConfig failed! Unknown line type!");
    }

}

ServerConfig::~ServerConfig()
{

}

ServerConfig& ServerConfig::operator = (const ServerConfig& From)
{
    Locations = From.Locations;
    IsDefaultServer = From.IsDefaultServer;
    ServerNames = From.ServerNames;
    ListenLocations = From.ListenLocations;

    // return the existing object so we can chain this operator
    return *this;
}

const LocationConfig& ServerConfig::GetLocation(const std::string& Search) const
{
    int Best = 0;
    size_t BestID = ~(size_t)0;

    for (size_t i = 0; i < Locations.size(); i++)
    {
        int Weight = Locations[i].GetMatchWeight(Search);
        if (Weight > Best)
        {
            Best = Weight;
            BestID = i;
        }
    }

    if (BestID == ~(size_t)0)
        throw LocationException(Search);

    return Locations[BestID];
}

bool ServerConfig::RepliesTo(const std::string& ServerName) const
{
    for (size_t i = 0; i < ServerNames.size(); i++)
        if (ServerNames[i] == ServerName)
            return true;
    return false;
}

bool ServerConfig::GetIsDefaultServer() const { return IsDefaultServer; }

std::ostream& operator<<(std::ostream& Stream, const ServerConfig& Server)
{
    if (Server.GetIsDefaultServer())
        Stream << "Default server!" << std::endl;;

    if (Server.ServerNames.size() > 0)
    {
        Stream << "Known as:" << std::endl;

        PrefixStreambuf Prefix(Stream, "  ");
        for (size_t i = 0; i < Server.ServerNames.size(); i++)
            Stream << Server.ServerNames[i] << std::endl;
        Prefix.End();
    }

    if (Server.ListenLocations.size() > 0)
    {
        Stream << "Listens to:" << std::endl;

        PrefixStreambuf Prefix(Stream, "  ");
        for (size_t i = 0; i < Server.ListenLocations.size(); i++)
            Stream << inet_ntoa(Server.ListenLocations[i].first) << ":" << ntohs(Server.ListenLocations[i].second) << std::endl;
        Prefix.End();
    }
    return Stream;
}
