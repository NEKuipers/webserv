class ConfigLine_location;	// For include loops

#ifndef CONFIGLINE_LOCATION_HPP
#define CONFIGLINE_LOCATION_HPP

#include <ostream>
#include "ConfigListBase.hpp"
#include "ConfigCombinedResponse.hpp"

class ConfigLine_location : public ConfigListBase {
	public: 
		ConfigLine_location();
		ConfigLine_location(const std::string& Location, const ConfigBlock& Block, const ConfigurationState &Configuration);

		virtual ~ConfigLine_location();

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigLine_location& ConfigLine_location);
		void Print(std::ostream& Stream) const;

		static ConfigLine_location* TryParse(const ConfigLine& Line, const ConfigurationState& Configuration);

		virtual ConfigResponse* GetIteratorResponse(std::vector<ConfigBase*>::const_iterator& It, const std::vector<ConfigBase*>::const_iterator& ItEnd, const ConfigRequest& Request, ConfigCombinedResponse& CombinedResponse) const;
	protected:
		virtual EnterResult Enters(const ConfigRequest& Request) const;
	private:
		int GetWeight() const;

		ConfigLine_location(const ConfigLine_location& From);
		ConfigLine_location& operator = (const ConfigLine_location& From);

		// Class variables
		std::string Location;

		// TODO: Option to remove location part from URI
		// TODO: Option to have it not error if nothing was found inside, and continue like it did not match

		// Class functions
};

#endif
