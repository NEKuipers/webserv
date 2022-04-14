#pragma once

#include "ConfigFile.hpp"
#include "ConfigListBase.hpp"

class Config : ConfigListBase {
	public:
		Config(const ConfigFile& File);

		~Config();

		friend std::ostream& operator<<(std::ostream& Stream, const Config& Config);

		ConfigResponse* GetResponse(const ConfigRequest& Request) const;
		std::vector<std::pair<in_addr_t, in_port_t> >* GetListenConnections();
	protected:
		virtual EnterResult Enters(const ConfigRequest& Request) const;

		// Public functions
	private:
		// No copying
		Config(const Config& From);
		Config& operator = (const Config& From);

		// Class variables

		// Class functions

};