class ConfigRedirectResponse;	// For include loops

#ifndef CONFIGREDIRECTRESPONSE_HPP
#define CONFIGREDIRECTRESPONSE_HPP

#include <ostream>
#include <fstream>
#include "ConfigResponse.hpp"
#include <map>
#include "Request.hpp"
#include <netinet/in.h>	// linux in_port_t

class ConfigRedirectResponse : public ConfigResponse {
	public:
		ConfigRedirectResponse(int Code, const std::string& NewPath, const ConfigErrorReasons& ErrorReasons);
		virtual ~ConfigRedirectResponse();

		// Public functions
		int GetCode() const;
		const std::string& GetNewPath() const;

		friend std::ostream& operator<<(std::ostream& Stream, const ConfigRedirectResponse& ConfigRedirectResponse);
		virtual void Print(std::ostream& Stream) const;
	private:
		// No, we dont copy, and we dont have a default constructor
		ConfigRedirectResponse();
		ConfigRedirectResponse(const ConfigRedirectResponse& From);
		ConfigRedirectResponse& operator = (const ConfigRedirectResponse& From);

		int Code;
		std::string NewPath;
};

#endif
