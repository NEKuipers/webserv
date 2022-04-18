#include "SimpleResponse.hpp"

SimpleResponse::SimpleResponse(const std::string &response_string) : response_string(response_string)
{

}


SimpleResponse::~SimpleResponse()
{
}

const std::string&			SimpleResponse::get_response_string()
{
	return response_string;
}

std::ostream& operator<<(std::ostream& Stream, const SimpleResponse& SimpleResponse)
{
	Stream << "SimpleResponse: " << SimpleResponse.response_string;
	return Stream;
}
