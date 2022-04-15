class ConfigFileResponse;	// For include loops

#ifndef CONFIGFILERESPONSE_HPP
#define CONFIGFILERESPONSE_HPP

#include <ostream>
#include <fstream>
#include "ConfigResponse.hpp"

class ConfigFileResponse : public ConfigResponse {
	public: 
		ConfigFileResponse(const std::string& FileName, std::ifstream* Stream, const ConfigErrorReasons& ErrorReasons);
		virtual ~ConfigFileResponse();

		// Public functions
		const std::ifstream& GetStream() const;

		friend std::ostream& operator<<(std::ostream& Stream, const ConfigFileResponse& ConfigFileResponse);
		virtual void Print(std::ostream& Stream) const;
		const std::string& GetContentType() const;
		const std::string& GetFileName();

		static void InitContentTypes();
	private:
		// No, we dont copy, and we dont have a default constructor
		ConfigFileResponse();
		ConfigFileResponse(const ConfigFileResponse& From);
		ConfigFileResponse& operator = (const ConfigFileResponse& From);

		// Class variables
		std::string FileName;	// Note: I only added this so it would print nice things to the console, so when writing the response into the socket only use 'Stream'
		std::ifstream* Stream;
		
		// Class functions
		
};

#endif
