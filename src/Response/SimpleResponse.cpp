#include "SimpleResponse.hpp"

SimpleResponse::SimpleResponse(const std::string &response_string) : response_string(response_string) 
{

}


SimpleResponse::~SimpleResponse()
{
}

bool			SimpleResponse::get_response_string(std::string &response_string)
{
	response_string = this->response_string;
	return true;
}

std::ostream& operator<<(std::ostream& Stream, const SimpleResponse& SimpleResponse)
{
	Stream << "SimpleResponse: " << SimpleResponse.response_string;
	return Stream;
}
