#include "Config.hpp"
#include "ConfigLine_try_file.hpp"
#include "ConfigLine_redirect.hpp"
#include "ConfigLine_server.hpp"
#include "ConvertException.hpp"

Config::Config(const ConfigFile& File)
{
	const std::vector<ConfigLine>& Lines = File.GetConfigLines();

	for (std::vector<ConfigLine>::const_iterator It = Lines.begin(); It != Lines.end(); It++)
	{
		const ConfigLine& Line = *It;
		if (Configuration.EatLine(Line)
		 || AddToChildren(ConfigLine_try_file::TryParse(Line, Configuration))
		 || AddToChildren(ConfigLine_redirect::TryParse(Line, Configuration))
		 || AddToChildren(ConfigLine_server  ::TryParse(Line, Configuration)))
			continue;
		
		throw ConvertException("ConfigLine", "Config", "Could not determine the meaning of line: '" + Line.GetArguments()[0] + "' in Config context");
	}
}

std::ostream& operator<<(std::ostream& Stream, const Config& Config)
{
	Config.Print(Stream);	// Will use the print of ConfigListBase
	return Stream;
}

Config::~Config()
{
	
}

EnterResult Config::Enters(const ConfigRequest& Request) const
{
	(void)Request;
	return EnterResult_EnterAndError;
}
