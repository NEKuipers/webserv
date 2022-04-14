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

ConfigResponse* Config::GetResponse(const ConfigRequest& Request) const
{
	ConfigCombinedResponse CombinedResponse;	// Why C++, Why can't i do 'ConfigCombinedResponse CombinedResponse()' WHATS THE DIFFERENCE!?
	return ConfigListBase::GetResponse(Request, CombinedResponse);
}

static void AddIfNew(std::vector<std::pair<in_addr_t, in_port_t> >* Vec, std::pair<in_addr_t, in_port_t> Pair)
{
	for (std::vector<std::pair<in_addr_t, in_port_t> >::const_iterator It = Vec->begin(); It != Vec->end(); It++)
		if (*It == Pair)
			return;	// Already exists

	Vec->push_back(Pair);
}

static void RecurseDescent(std::vector<std::pair<in_addr_t, in_port_t> >* Vec, ConfigBase* Current, bool InsideServerBlock = false)
{
	ConfigLine_server* ServerLine = dynamic_cast<ConfigLine_server*>(Current);
	if (ServerLine)
	{
		const std::vector<std::pair<in_addr_t, in_port_t> >& Listens = ServerLine->GetListens();
		InsideServerBlock = Listens.size() > 0;	// If we dont listen to any port, dont add the default yet, in case there is a server block inside this server block, or there are no responses inside we dont add it

		for (std::vector<std::pair<in_addr_t, in_port_t> >::const_iterator It = Listens.begin(); It != Listens.end(); It++)
			AddIfNew(Vec, *It);

		// Dont return quickly, maybe there are server blocks inside this server block
	}

	ConfigListBase* ListBase = dynamic_cast<ConfigListBase*>(Current);
	if (ListBase)
	{
		const std::vector<ConfigBase*>& Children = ListBase->GetChildren();

		for (std::vector<ConfigBase*>::const_iterator It = Children.begin(); It != Children.end(); It++)
			RecurseDescent(Vec, *It, InsideServerBlock);
		return;
	}

	// Okay, well, This is a response block, If we are inside a server block then everything is good, otherwise add the default host:port pair
	if (InsideServerBlock)
		return;

	AddIfNew(Vec, std::make_pair<in_addr_t, in_port_t>(INADDR_ANY, htons(ConfigLine_server::DEFAULT_PORT)));
}

// I would love a move operator right here, dont have to allocate the vector on the heap then
std::vector<std::pair<in_addr_t, in_port_t> >* Config::GetListenConnections()
{
	std::vector<std::pair<in_addr_t, in_port_t> >* Vec = new std::vector<std::pair<in_addr_t, in_port_t> >();
	if (!Vec)
		return NULL;

	RecurseDescent(Vec, this);
	return Vec;
}