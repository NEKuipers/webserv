#include "ConfigLine_location.hpp"
#include "ConvertException.hpp"

ConfigLine_location::ConfigLine_location() {}
ConfigLine_location::ConfigLine_location(const std::string& Location, const ConfigBlock& Block, const ConfigurationState &Configuration) : ConfigListBase(Configuration), Location(Location)
{
	ReadBlock("ConfigLine_location", BaseLines, Block.GetLines());
}
ConfigLine_location::~ConfigLine_location()
{
	
}

std::ostream& operator<<(std::ostream& Stream, const ConfigLine_location& ConfigLine_location)
{
	ConfigLine_location.Print(Stream);
	return Stream;
}
void ConfigLine_location::Print(std::ostream& Stream) const
{
	Stream << "Location: " << Location << "/ ";
	ConfigListBase::Print(Stream);
}

ConfigLine_location* ConfigLine_location::TryParse(const ConfigLine& Line, const ConfigurationState& Configuration)
{
	std::vector<std::string> Args = Line.GetArguments();
	if (Args.at(0) != "location")
		return NULL;

	if (Args.size() != 2)
		throw ConvertException("ConfigLine", "ConfigLine_location", "Bad argument count! Expected 2, Got " + std::to_string(Args.size()));

	ConfigBlock* Block = Line.GetBlock();
	if (Block == NULL)
		throw ConvertException("ConfigLine", "ConfigLine_location", "Missing block!");

	std::string Location = Args.at(1);

	if (Location.at(Location.length() - 1) != '/')
		throw ConvertException("ConfigLine", "ConfigLine_location", "Location must ALWAYS end with /");
	Location = Location.substr(0, Location.length() - 1);	// Remove trailing /

	return new ConfigLine_location(Location, *Block, Configuration);
}

EnterResult ConfigLine_location::Enters(const ConfigRequest& Request) const
{
	const std::string& URI = Request.GetUri();

	if (URI.rfind(Location, 0) == std::string::npos)	// URI.starts_with(Location)
		return EnterResult_No;

	return EnterResult_EnterAndError;
}