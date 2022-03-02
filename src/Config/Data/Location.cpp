#include "Location.hpp"
#include "ConvertException.hpp"

const std::string Location::DESCRIPTOR = "location";

Location::Location() {}
Location::Location(const Location& From)
{
    this->operator=(From);
}

Location::Location(const ConfigLine& Line)
{
    const std::vector<std::string>& Args = Line.GetArguments();

    if (Args.size() != 2)
        throw ConvertException("Converting from Line to Location failed! Args.size() != 2");
    if (Args[0] != DESCRIPTOR)
        throw ConvertException("Converting from Line to Location failed! Args[0] != DESCRIPTOR");
    
    Match = Args[1];
    if (Match.at(Match.size() - 1) != '\\')
        throw ConvertException("Converting from Line to Location failed! Args[1] must end with a \\");
    
    Match = Match.substr(0, Match.length() - 1);

    ConfigBlock* Block = Line.GetBlock();
    if (Block == NULL)
        throw ConvertException("Converting from Line to Location failed! Must contain a block!");

    // TODO: Do something with the block
}

Location::~Location()
{
    
}

Location& Location::operator = (const Location& From)
{
    this->Match = From.Match;

    // return the existing object so we can chain this operator
    return *this;
}

int Location::GetMatchWeight(const std::string& MatchAgainst) const
{
    if (Match.find(MatchAgainst) != std::string::npos)
        return Match.length();
    return 0;
}
