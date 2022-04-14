class ErrorResponse;	// For include loops

#ifndef ERRORRESPONSE_HPP
#define ERRORRESPONSE_HPP

#include <ostream>
#include "ConfigResponse.hpp"

class ErrorResponse : public ConfigResponse {
	public: 
		ErrorResponse(const ConfigErrorReasons& ErrorReasons);
		~ErrorResponse();

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ErrorResponse& ErrorResponse);
		virtual void Print(std::ostream& Stream) const;
	private:
		ErrorResponse();
		ErrorResponse(const ErrorResponse& From);
		ErrorResponse& operator = (const ErrorResponse& From);
		// Class variables
		
		// Class functions
		
};

#endif
