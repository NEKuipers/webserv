class ConfigDirectoryResponse;	// For include loops

#ifndef CONFIGDIRECTORYRESPONSE_HPP
#define CONFIGDIRECTORYRESPONSE_HPP

#include <ostream>
#include <fstream>
#include "ConfigResponse.hpp"

class ConfigDirectoryResponse : public ConfigResponse {
	public: 
		ConfigDirectoryResponse(const std::string& Directory, const ConfigErrorReasons& ErrorReasons);
		virtual ~ConfigDirectoryResponse();

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigDirectoryResponse& ConfigDirectoryResponse);
		virtual void Print(std::ostream& Stream) const;
		const std::string& GetDirectory();
	private:
		// No, we dont copy, and we dont have a default constructor
		ConfigDirectoryResponse();
		ConfigDirectoryResponse(const ConfigDirectoryResponse& From);
		ConfigDirectoryResponse& operator = (const ConfigDirectoryResponse& From);

		// Class variables
		std::string Directory;
		
		// Class functions
		
};

#endif

