class ConfigDeleteResponse;	// For include loops

#ifndef CONFIGDELETERESPONSE_HPP
#define CONFIGDELETERESPONSE_HPP

#include <ostream>
#include <fstream>
#include "ConfigResponse.hpp"
#include <map>
#include "Request.hpp"
#include <netinet/in.h>	// linux in_port_t

class ConfigDeleteResponse : public ConfigResponse {
	public:
		ConfigDeleteResponse(const std::string& DeleteFile, const ConfigErrorReasons& ErrorReasons);
		virtual ~ConfigDeleteResponse();

		// Public functions
		const std::string& GetDeleteFile() const;

		friend std::ostream& operator<<(std::ostream& Stream, const ConfigDeleteResponse& ConfigDeleteResponse);
		virtual void Print(std::ostream& Stream) const;
	private:
		// No, we dont copy, and we dont have a default constructor
		ConfigDeleteResponse();
		ConfigDeleteResponse(const ConfigDeleteResponse& From);
		ConfigDeleteResponse& operator = (const ConfigDeleteResponse& From);

		std::string DeleteFile;
};

#endif
