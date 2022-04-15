class CGIResponse;	// For include loops

#ifndef CGIResponse_HPP
#define CGIResponse_HPP
#include "Response.hpp"
#include "CGIRunner.hpp"
#include <ostream>

class CGIResponse : public Response
{
	public: 
		CGIResponse(CGIRunner *cgiRunner);

		~CGIResponse();


		// Public functions
		virtual bool			get_response_string(std::string &response_string);
		const CGIRunner			*get_cgi_runner() const;

		friend std::ostream& operator<<(std::ostream& Stream, const CGIResponse& CGIResponse);
		
	private:
		// Class variables
		CGIRunner *cgiRunner;
		// Class functions
		CGIResponse();
		CGIResponse(const CGIResponse& From);
		CGIResponse& operator = (const CGIResponse& From);
};

#endif
