class ConfigUploadFileResponse;	// For include loops

#ifndef CONFIGUPLOADFILERESPONSE_HPP
#define CONFIGUPLOADFILERESPONSE_HPP

#include <ostream>
#include <fstream>
#include "ConfigResponse.hpp"

class ConfigUploadFileResponse : public ConfigResponse {
	public: 
		ConfigUploadFileResponse(const std::string& FileName, const ConfigErrorReasons& ErrorReasons);
		virtual ~ConfigUploadFileResponse();

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigUploadFileResponse& ConfigUploadFileResponse);
		virtual void Print(std::ostream& Stream) const;
		const std::string& GetFileName();

		virtual bool RequiresBody() const;
	private:
		// No, we dont copy, and we dont have a default constructor
		ConfigUploadFileResponse();
		ConfigUploadFileResponse(const ConfigUploadFileResponse& From);
		ConfigUploadFileResponse& operator = (const ConfigUploadFileResponse& From);

		// Class variables
		std::string FileName;	// Note: I only added this so it would print nice things to the console, so when writing the response into the socket only use 'Stream'
		
		// Class functions
		
};

#endif
