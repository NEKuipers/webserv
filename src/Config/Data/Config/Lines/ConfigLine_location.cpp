#include "ConfigLine_location.hpp"
#include "ConvertException.hpp"

#include "ToString.hpp"

ConfigLine_location::ConfigLine_location() {}
ConfigLine_location::ConfigLine_location(const std::string& Location, const ConfigBlock& Block, const ConfigurationState &Configuration) : ConfigListBase(Configuration), Location(Location)
{
	if (Location.size() > 0 && Location.at(0) != '*')
		this->Configuration.AppendLocationRoot(Location);
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
		throw ConvertException("ConfigLine", "ConfigLine_location", "Bad argument count! Expected 2, Got " + to_string(Args.size()));

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

	if (Location.length() > 0 && ((Location.at(0) == '*')	// I mean, routes wont be using regex, but that does not mean i can't do something simple like this, right?
		? (URI.find(Location.c_str() + 1, URI.length() - Location.length()) == std::string::npos)	// URI.ends_with(Location+1)
		: (URI.rfind(Location, 0) == std::string::npos)	// URI.starts_with(Location)
	))
			return EnterResult_No;

	return EnterResult_EnterAndError;
}

int ConfigLine_location::GetWeight() const
{
	if (Location.length() > 0 && Location.at(0) == '*')
		return INT_MAX;
	else
		return Location.length();
}

ConfigResponse* ConfigLine_location::GetIteratorResponse(std::vector<ConfigBase*>::const_iterator& It, const std::vector<ConfigBase*>::const_iterator& ItEnd, const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const
{
	const ConfigLine_location* Best = NULL;

	while (It != ItEnd)
	{
		const ConfigLine_location* Curr = dynamic_cast<const ConfigLine_location*>(*It);
		if (!Curr)
			break;
		It++;

		EnterResult Enters = Curr->Enters(Request);
		if (Enters == EnterResult_No)
			continue;
		if (Curr->ChecksConfiguration() && !Curr->Configuration.IsValidWithRequest(Request))
			continue;
		if (Best == NULL || Best->GetWeight() < Curr->GetWeight())
			Best = Curr;
	}

	if (!Best)
		return NULL;
	return Best->GetBaseResponse(Request, ErrorReasons);
}