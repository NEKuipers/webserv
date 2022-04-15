#include "CGIResponse.hpp"

CGIResponse::CGIResponse(CGIRunner *cgiRunner) : cgiRunner(cgiRunner)
{

}

CGIResponse::~CGIResponse()
{
	// TODO: Implement destructor
}

bool			CGIResponse::get_response_string(std::string &response_string)
{
	return cgiRunner->Read(response_string);
}

const CGIRunner			*CGIResponse::get_cgi_runner() const
{
	return cgiRunner;
}


std::ostream& operator<<(std::ostream& Stream, const CGIResponse& CGIResponse)
{
	Stream << "CGIResponse " << * CGIResponse.get_cgi_runner();
	return Stream;
}