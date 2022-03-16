#include "ParseRequestException.hpp"

ParseRequestException::ParseRequestException(){}

ParseRequestException::~ParseRequestException() _NOEXCEPT {}

const char* ParseRequestException::what () const throw () {
	return "Failed to parse http request!";
}
