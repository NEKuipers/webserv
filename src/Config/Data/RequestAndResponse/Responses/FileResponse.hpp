class FileResponse;	// For include loops

#ifndef FILERESPONSE_HPP
#define FILERESPONSE_HPP

#include <ostream>
#include <fstream>
#include "ConfigResponse.hpp"

class FileResponse : public ConfigResponse {
	public: 
		FileResponse(const std::string& FileName, std::ifstream* Stream, const ConfigErrorReasons& ErrorReasons);
		virtual ~FileResponse();

		// Public functions
		const std::ifstream& GetStream() const;

		friend std::ostream& operator<<(std::ostream& Stream, const FileResponse& FileResponse);
		virtual void Print(std::ostream& Stream) const;
		const std::string GetContentType() const;
		const std::string GetFileName();

		static void InitContentTypes();
	private:
		// No, we dont copy, and we dont have a default constructor
		FileResponse();
		FileResponse(const FileResponse& From);
		FileResponse& operator = (const FileResponse& From);

		// Class variables
		std::string FileName;	// Note: I only added this so it would print nice things to the console, so when writing the response into the socket only use 'Stream'
		std::ifstream* Stream;
		
		// Class functions
		
};

#endif
