#include "LocationConfig.hpp"
#include "ConvertException.hpp"

const std::string LocationConfig::DESCRIPTOR = "location";

LocationConfig::LocationConfig() {}
LocationConfig::LocationConfig(const LocationConfig& From)
{
    this->operator=(From);
}

LocationConfig::LocationConfig(const ConfigLine& Line)
{
    const std::vector<std::string>& Args = Line.GetArguments();

    if (Args.size() != 2)
        throw ConvertException("Converting from Line to LocationConfig failed! Args.size() != 2");
    if (Args[0] != DESCRIPTOR)
        throw ConvertException("Converting from Line to LocationConfig failed! Args[0] != DESCRIPTOR");

    Match = Args[1];
    if (Match.at(Match.size() - 1) != '\\')
        throw ConvertException("Converting from Line to LocationConfig failed! Args[1] must end with a \\");

    Match = Match.substr(0, Match.length() - 1);

    ConfigBlock* Block = Line.GetBlock();
    if (Block == NULL)
        throw ConvertException("Converting from Line to LocationConfig failed! Must contain a block!");

    // TODO: Do something with the block
}

LocationConfig::~LocationConfig()
{

}

LocationConfig& LocationConfig::operator = (const LocationConfig& From)
{
    this->Match = From.Match;

    // return the existing object so we can chain this operator
    return *this;
}

int LocationConfig::GetMatchWeight(const std::string& MatchAgainst) const
{
    if (Match.find(MatchAgainst) != std::string::npos)
        return Match.length();
    return 0;
}
