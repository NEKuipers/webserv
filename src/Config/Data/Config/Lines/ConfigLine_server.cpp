#include "ConfigLine_server.hpp"
#include "PrefixStreambuf.hpp"
#include "ConfigBlock.hpp"
#include "ConvertException.hpp"

#include "ConfigLine_try_file.hpp"
#include "ConfigLine_redirect.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

ConfigLine_server::ConfigLine_server() {}
ConfigLine_server::ConfigLine_server(const ConfigBlock* Block, const ConfigurationState &Configuration_) : ConfigListBase(Configuration_)
{
	ReadBlock("ConfigLine_server", BaseLines, Block->GetLines());
	
	// TODO: Re-order location blocks in order of longest match length
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
		throw ConvertException("ConfigLine", "ConfigLine_server", "Bad argument count! Expected 1, Got " + std::to_string(Args.size()));

	ConfigBlock* Block = Line.GetBlock();
	if (Block == NULL)
		throw ConvertException("ConfigLine", "ConfigLine_server", "Missing block!");
	
	return new ConfigLine_server(Block, Configuration);
}

EnterResult ConfigLine_server::Enters(const ConfigRequest& Request) const
{
	if (ServerNames.size() > 0)
	{
		bool HasMatch = false;
		for (std::vector<std::string>::const_iterator It = ServerNames.begin(); It != ServerNames.end(); It++)
			if (Request.GetServerName() == *It)
			{
				HasMatch = true;
				break;
			}
		if (!HasMatch)
			return EnterResult_No;
	}
	if (Listens.size() > 0)
	{
		bool HasMatch = false;
		for (std::vector<std::pair<in_addr_t, in_port_t> >::const_iterator It = Listens.begin(); It != Listens.end(); It++)
			if (Request.GetAddr() == It->first && Request.GetPort() == It->second)
			{
				HasMatch = true;
				break;
			}
		if (!HasMatch)
			return EnterResult_No;
	}

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

			in_port_t Port = DEFAULT_PORT;
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
