#include "ConfigLine_index.hpp"
#include "ConvertException.hpp"

#include "ToString.hpp"

ConfigLine_index::ConfigLine_index() {}
ConfigLine_index::ConfigLine_index(const ConfigBlock& Block, const ConfigurationState &Configuration) : ConfigListBase(Configuration)
{
	ReadBlock("ConfigLine_index", BaseLines, Block.GetLines());
}
ConfigLine_index::~ConfigLine_index()
{

}

std::ostream& operator<<(std::ostream& Stream, const ConfigLine_index& ConfigLine_index)
{
	ConfigLine_index.Print(Stream);
	return Stream;
}
void ConfigLine_index::Print(std::ostream& Stream) const
{
	Stream << "Index: " << "/ ";
	ConfigListBase::Print(Stream);
}

ConfigLine_index* ConfigLine_index::TryParse(const ConfigLine& Line, const ConfigurationState& Configuration)
{
	std::vector<std::string> Args = Line.GetArguments();
	if (Args.at(0) != "index")
		return NULL;

	if (Args.size() != 1)
		throw ConvertException("ConfigLine", "ConfigLine_index", "Bad argument count! Expected 1, Got " + to_string(Args.size()));

	ConfigBlock* Block = Line.GetBlock();
	if (Block == NULL)
		throw ConvertException("ConfigLine", "ConfigLine_index", "Missing block!");

	return new ConfigLine_index(*Block, Configuration);
}

EnterResult ConfigLine_index::Enters(const ConfigRequest& Request) const
{
	const std::string& URI = Request.GetUri();

	// URI must be equal to [ExpectedRootExtension]

	// URI can be "" or "test/"
	// ExpectedRootExtension can be "" or "/data"
	// We dont care about the slashes
	
	if (Configuration.ExpectedRootExtension == URI	// If both are empty
	|| Configuration.ExpectedRootExtension + "/" == "/" + URI)	// Or both are filled
		return EnterResult_EnterAndError;

	return EnterResult_No;
}
