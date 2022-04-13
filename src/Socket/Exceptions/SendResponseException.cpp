#include "SendResponseException.hpp"

SendResponseException::SendResponseException() {}

SendResponseException::~SendResponseException() throw()
{}

const char* SendResponseException::what() const throw()
{
	return "Error: could not write to socket properly.";
}
std::ostream& operator<<(std::ostream& Stream, const SendResponseException& SendResponseException)
{
	Stream << SendResponseException.what(); 
	return Stream;
}
