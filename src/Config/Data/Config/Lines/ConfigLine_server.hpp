class ConfigLine_server;	// For include loops

#ifndef CONFIGLINE_SERVER_HPP
#define CONFIGLINE_SERVER_HPP

#include <ostream>
#include "ConfigListBase.hpp"
#include <vector>

class ConfigLine_server : public ConfigListBase {
	public: 
		static const in_port_t DEFAULT_PORT = htons(80);

		ConfigLine_server();
		ConfigLine_server(const ConfigBlock* Block, const ConfigurationState &Configuration);

		virtual ~ConfigLine_server();

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigLine_server& ConfigLine_server);
		void Print(std::ostream& Stream) const;
		
		static ConfigLine_server* TryParse(const ConfigLine& Line, const ConfigurationState& Configuration);
	protected:
		virtual EnterResult Enters(const ConfigRequest& Request) const;
	private:
		// No copying
		ConfigLine_server(const ConfigLine_server& From);
		ConfigLine_server& operator = (const ConfigLine_server& From);

		// Class variables
		std::vector<std::string> ServerNames;
		std::vector<std::pair<in_addr_t, in_port_t> > Listens;
		
		// Class functions
		bool EatLine(const ConfigLine& Line);
};

#endif
