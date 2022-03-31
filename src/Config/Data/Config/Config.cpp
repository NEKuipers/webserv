#include "Config.hpp"
#include "ConfigLine_try_file.hpp"
#include "ConfigLine_redirect.hpp"
#include "ConfigLine_server.hpp"
#include "ConvertException.hpp"

Config::Config(const ConfigFile& File)
{
	ReadBlock("Config", BaseLines, File.GetConfigLines());
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
