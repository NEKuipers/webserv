#include "Server.hpp"
#include "ConvertException.hpp"
#include "LocationException.hpp"

Server::Server() {}
Server::Server(const Server& From)
{
    this->operator=(From);
}

Server::Server(const ConfigLine& Line) : Locations(), IsDefaultServer(false), ServerNames()
{
    const std::vector<std::string>& Args = Line.GetArguments();

    if (Args.size() != 1)
        throw ConvertException("Converting from Line to Server failed! Args.size() != 1");
    if (Args[0] != DESCRIPTOR)
        throw ConvertException("Converting from Line to Server failed! Args[0] != DESCRIPTOR");

    ConfigBlock* Block = Line.GetBlock();
    if (Block == NULL)
        throw ConvertException("Converting from Line to Location failed! Must contain a block!");

    const std::vector<ConfigLine>& Lines = Block->GetLines();
    for (size_t i = 0; i < Lines.size(); i++)
    {
        // TODO: More types

        // TODO: IsDefaultServer

        const std::vector<std::string>& LineArgs = Lines[i].GetArguments();
        if (LineArgs.size() <= 0)
            throw ConvertException("Converting from Line to Location failed! Block contained empty line? Should be impossible!");
        

        // TODO: This if/else if/else if/else thing is going to be huge!
        //  Find a neat way to do it properly
        if (LineArgs[0] == Location::DESCRIPTOR)
            Locations.push_back(Location(Lines[i]));
        else if (LineArgs[0] == "server_name")
            ServerNames.insert(ServerNames.end(), LineArgs.begin() + 1, LineArgs.end());
        else
            throw ConvertException("Converting from Line to Location failed! Unknown line type!");
    }
    
}

Server::~Server()
{
    
}

Server& Server::operator = (const Server& From)
{
    Locations = From.Locations;

    // return the existing object so we can chain this operator
    return *this;
}

const Location& Server::GetLocation(const std::string& Search)
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

bool Server::RepliesTo(const std::string& ServerName)
{
    for (size_t i = 0; i < ServerNames.size(); i++)
        if (ServerNames[i] == ServerName)
            return true;
    return false;
}

bool Server::GetIsDefaultServer() { return IsDefaultServer; }
