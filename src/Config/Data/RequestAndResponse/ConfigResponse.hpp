class ConfigResponse;	// For include loops

#ifndef CONFIGRESPONSE_HPP
#define CONFIGRESPONSE_HPP

#include <ostream>
#include <vector>
#include "ConfigCombinedResponse.hpp"

// Note: This is a abstract class, and a abstract function should exist that takes in the http request, socket, and writes the response into that
class ConfigResponse {
	public: 
		ConfigResponse(const ConfigCombinedResponse& ConfigCombinedResponse);
		virtual ~ConfigResponse();
		

		virtual void Print(std::ostream& Stream) const = 0;	// Apparently this is how you do virtual logging?

		virtual bool RequiresBody() const;

		const std::vector<std::string>& GetAllowedMethods();
	private:
		// No copying or default constructing
		ConfigResponse();
		ConfigResponse(const ConfigResponse& From);
		ConfigResponse& operator = (const ConfigResponse& From);
		// Class variables
		std::vector<std::string> AllowedMethods;
		// Class functions
		
};

#endif
