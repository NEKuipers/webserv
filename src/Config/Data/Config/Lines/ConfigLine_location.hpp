class ConfigLine_location;	// For include loops

#ifndef CONFIGLINE_LOCATION_HPP
#define CONFIGLINE_LOCATION_HPP

#include <ostream>
#include "ConfigListBase.hpp"
#include "ConfigErrorReasons.hpp"

class ConfigLine_location : public ConfigListBase {
	public: 
		ConfigLine_location();
		ConfigLine_location(const std::string& Location, const ConfigBlock& Block, const ConfigurationState &Configuration);

		virtual ~ConfigLine_location();

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigLine_location& ConfigLine_location);
		virtual void Print(std::ostream& Stream) const;

		static ConfigLine_location* TryParse(const ConfigLine& Line, const ConfigurationState& Configuration);

		virtual ConfigResponse* GetIteratorResponse(std::vector<ConfigBase*>::const_iterator& It, const std::vector<ConfigBase*>::const_iterator& ItEnd, const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const;
	protected:
		virtual EnterResult Enters(const ConfigRequest& Request) const;
	private:
		int GetWeight() const;

		ConfigLine_location(const ConfigLine_location& From);
		ConfigLine_location& operator = (const ConfigLine_location& From);

		// Class variables
		std::string Location;

		// Class functions
};

#endif
