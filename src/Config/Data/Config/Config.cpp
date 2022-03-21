#include "Config.hpp"
#include "ConfigLine_try_file.hpp"
#include "ConfigLine_redirect.hpp"
#include "ConvertException.hpp"

static bool AddToChildren(std::vector<ConfigBase*>& Children, ConfigBase* ConfigBase)
{
	if (!ConfigBase)
		return false;
	Children.push_back(ConfigBase);
	return true;
}

Config::Config(const ConfigFile& File)
{
	const std::vector<ConfigLine>& Lines = File.GetConfigLines();

	for (std::vector<ConfigLine>::const_iterator It = Lines.begin(); It != Lines.end(); It++)
	{
		const ConfigLine& Line = *It;
		if (Line.IsComment()
		 || Configuration.EatLine(Line)
		 || AddToChildren(Children, ConfigLine_try_file::TryParse(Line, Configuration))
		 || AddToChildren(Children, ConfigLine_redirect::TryParse(Line, Configuration)))
			continue;
		
		throw ConvertException("Could not determine the meaning of line: '" + Line.GetArguments()[0] + "'");
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
