#include "ConfigLine_server.hpp"
#include "PrefixStreambuf.hpp"
#include "ConfigBlock.hpp"
#include "ConvertException.hpp"

#include "ConfigLine_try_file.hpp"
#include "ConfigLine_redirect.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ToString.hpp"
#include <cstdlib>	// linux strtoul

ConfigLine_server::ConfigLine_server() {}
ConfigLine_server::ConfigLine_server(const ConfigBlock& Block, const ConfigurationState &Configuration_) : ConfigListBase(Configuration_), ServerNames(), Listens()
{
	ReadBlock("ConfigLine_server", BaseLines, Block.GetLines());
}

ConfigLine_server::~ConfigLine_server()
{

}

std::ostream& operator<<(std::ostream& Stream, const ConfigLine_server& ConfigLine_server)
{
	ConfigLine_server.Print(Stream);
	return Stream;
}
void ConfigLine_server::Print(std::ostream& Stream) const
{
	Stream << "Server:\n";
	PrefixStreambuf Prefix(Stream, "  ");

	if (ServerNames.size() > 0)
	{
		Stream << "Known as:\n";
		PrefixStreambuf NamePrefix(Stream, "  ");
		for (std::vector<std::string>::const_iterator It = ServerNames.begin(); It != ServerNames.end(); It++)
			Stream << *It << "\n";
	}

	if (Listens.size() > 0)
	{
		Stream << "Listens to:\n";
		PrefixStreambuf ListenPrefix(Stream, "  ");
		for (std::vector<std::pair<in_addr_t, in_port_t> >::const_iterator It = Listens.begin(); It != Listens.end(); It++)
		{
			in_addr Addr;
			Addr.s_addr = (*It).first;
			Stream << inet_ntoa(Addr) << ":" << ntohs((*It).second) << "\n";
		}
	}

	Stream << "Children: ";
	ConfigListBase::Print(Stream);
}

ConfigLine_server* ConfigLine_server::TryParse(const ConfigLine& Line, const ConfigurationState& Configuration)
{
	std::vector<std::string> Args = Line.GetArguments();
	if (Args.at(0) != "server")
		return NULL;

	if (Args.size() != 1)
		throw ConvertException("ConfigLine", "ConfigLine_server", "Bad argument count! Expected 1, Got " + to_string(Args.size()));

	ConfigBlock* Block = Line.GetBlock();
	if (Block == NULL)
		throw ConvertException("ConfigLine", "ConfigLine_server", "Missing block!");

	return new ConfigLine_server(*Block, Configuration);
}

const std::vector<std::pair<in_addr_t, in_port_t> >& ConfigLine_server::GetListens() { return Listens; }

bool ConfigLine_server::MatchesServerName(const ConfigRequest& Request) const
{
	for (std::vector<std::string>::const_iterator It = ServerNames.begin(); It != ServerNames.end(); It++)
		if (Request.GetServerName() == *It)
			return true;
	return false;
}
bool ConfigLine_server::MatchesIP(const ConfigRequest& Request) const
{
	for (std::vector<std::pair<in_addr_t, in_port_t> >::const_iterator It = Listens.begin(); It != Listens.end(); It++)
		if (Request.GetAddr() == It->first && Request.GetPort() == It->second)
			return true;
	return false;
}

EnterResult ConfigLine_server::Enters(const ConfigRequest& Request) const
{
	(void)Request;

	// Due to overloaded GetIteratorResponse we know already that we should enter
	/*
	if (ServerNames.size() > 0)
	{
		if (!MatchesServerName(Request))
			return EnterResult_No;
	}
	if (Listens.size() > 0)
	{
		if (!MatchesIP(Request))
			return EnterResult_No;
	}
	*/

	return EnterResult_EnterAndError;
}

bool ConfigLine_server::EatLine(const ConfigLine& Line)
{
	std::vector<std::string> Args = Line.GetArguments();

	if (Args.at(0) == "server_name" || Args.at(0) == "server_names")
		ServerNames.insert(ServerNames.end(), Args.begin() + 1, Args.end());
	else if (Args.at(0) == "listen" || Args.at(0) == "listens")
	{
		for (std::vector<std::string>::const_iterator It = Args.begin() + 1; It != Args.end(); It++)
		{
			std::string AddrStr = *It;

			in_port_t Port = htons(DEFAULT_PORT);
			size_t split = AddrStr.find(":");
			if (split != std::string::npos)
			{
				char* End;
				unsigned long PortUL = std::strtoul(&AddrStr.c_str()[split + 1], &End, 10);
				Port = (in_port_t)PortUL;

				if (Port != PortUL) // Out of bounds checking
					throw ConvertException("ConfigLine", "Listen Port", "Port too big");
				else if (End != &AddrStr.c_str()[AddrStr.length()]) // Check if the string contained more than just a number
					throw ConvertException("ConfigLine", "Listen Port", "Port contained more than just a number");

				AddrStr = AddrStr.substr(0, split);
				Port = htons(Port);
			}

			in_addr Addr;
			if (inet_aton(AddrStr.c_str(), &Addr) == 0)
				throw ConvertException("ConfigLine", "Listen Adress", "Malformed Adress");

			Listens.push_back(std::make_pair(Addr.s_addr, Port));
		}
	}
	else
		return false;
	return true;
}

ConfigResponse* ConfigLine_server::GetIteratorResponse(std::vector<ConfigBase*>::const_iterator& It, const std::vector<ConfigBase*>::const_iterator& ItEnd, const ConfigRequest& Request) const
{
	const ConfigLine_server* Default = NULL;
	bool DefaultIpMatch = false;

	// Basically:
	//	The first one that matches the server_name is the default, otherwise the first one that matches the ip, and otherwise the first wildcard ip
	//	If there is one that matches both the IP AND server name, instantly return that

	// So the priority is:
	//	IP && NAME
	//	NAME
	//	IP
	//	Wildcard IP

	while (It != ItEnd)
	{
		const ConfigLine_server* Curr = dynamic_cast<const ConfigLine_server*>(*It);
		if (!Curr)
			break;
		It++;

		bool IpMatch = Curr->MatchesIP(Request);
		if (!IpMatch && Curr->Listens.size() > 0)
			continue;	// Ip does not match, but a ip was specified, this server does not respond.

		if (Default == NULL || (IpMatch && !DefaultIpMatch))
		{
			Default = Curr;	// Set the default, first server that matches ip
			DefaultIpMatch = IpMatch;
		}

		if (Curr->MatchesServerName(Request))
		{
			if (IpMatch)
				return Curr->GetBaseResponse(Request);	// Well, IP matches, and server name matches, there is no way we have a server that has a higher priority
			else
			{
				Default = Curr;	// wildcard IP and matching server name? set new default
				DefaultIpMatch = true;	// only matching server name has more priority than only matching ip
			}
		}
	}

	if (!Default)
		return NULL;
	return Default->GetBaseResponse(Request);
}