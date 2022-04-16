class SimpleResponse;	// For include loops

#ifndef SimpleResponse_HPP
#define SimpleResponse_HPP
#include "Response.hpp"
#include <ostream>

class SimpleResponse : public Response
{
	public:
		SimpleResponse(const std::string &http_response);

		virtual ~SimpleResponse();


		const std::string&		get_response_string();
		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const SimpleResponse& SimpleResponse);

	private:
		// Class variables
		std::string response_string;
		// Class functions
		SimpleResponse();
		SimpleResponse(const SimpleResponse& From);
		SimpleResponse& operator = (const SimpleResponse& From);
};

#endif
