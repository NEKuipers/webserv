#pragma once

#include "ConfigFile.hpp"
#include "ConfigListBase.hpp"

class Config : public ConfigListBase {
	public: 
		Config(const ConfigFile& File);
		Config(const Config& From);

		~Config();

		Config& operator = (const Config& From);

		friend std::ostream& operator<<(std::ostream& Stream, const Config& Config);
	protected:
		virtual EnterResult Enters(const ConfigRequest& Request) const;
		
		// Public functions
	private:
		// Class variables
		
		// Class functions
		
};