#pragma once

#include "ConfigFile.hpp"
#include "ConfigListBase.hpp"

class Config : public ConfigListBase {
	public:
		Config(const ConfigFile& File);

		~Config();


		friend std::ostream& operator<<(std::ostream& Stream, const Config& Config);
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