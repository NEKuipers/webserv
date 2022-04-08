class CgiResponse;	// For include loops

#ifndef CGIRESPONSE_HPP
#define CGIRESPONSE_HPP

#include <ostream>
#include <fstream>
#include "ConfigResponse.hpp"

class CgiResponse : public ConfigResponse {
	public: 
		CgiResponse(const std::string& CgiFile);
		virtual ~CgiResponse();

		// Public functions
		const std::string& GetCgiFile() const;

		friend std::ostream& operator<<(std::ostream& Stream, const CgiResponse& CgiResponse);
		virtual void Print(std::ostream& Stream) const;

		virtual bool RequiresBody() const;
	private:
		// No, we dont copy, and we dont have a default constructor
		CgiResponse();
		CgiResponse(const CgiResponse& From);
		CgiResponse& operator = (const CgiResponse& From);

		// Class variables
		std::string CgiFile;
		
		// Class functions
		
};

#endif
