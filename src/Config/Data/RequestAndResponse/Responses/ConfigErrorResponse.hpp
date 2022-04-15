class ConfigErrorResponse;	// For include loops

#ifndef CONFIGERRORRESPONSE_HPP
#define CONFIGERRORRESPONSE_HPP

#include <ostream>
#include "ConfigResponse.hpp"

class ConfigErrorResponse : public ConfigResponse {
	public: 
		ConfigErrorResponse(const ConfigErrorReasons& ErrorReasons);
		~ConfigErrorResponse();

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigErrorResponse& ConfigErrorResponse);
		virtual void Print(std::ostream& Stream) const;
	private:
		ConfigErrorResponse();
		ConfigErrorResponse(const ConfigErrorResponse& From);
		ConfigErrorResponse& operator = (const ConfigErrorResponse& From);
		// Class variables
		
		// Class functions
		
};

#endif
