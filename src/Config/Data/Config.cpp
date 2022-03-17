#include "Config.hpp"

Config::Config(const ConfigFile& File) : RequestResponderList(ConfigurationState())
{
	// TODO: read out infos
	(void)File;
}

Config::~Config()
{
	// TODO: Implement destructor
}