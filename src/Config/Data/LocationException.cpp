#include "LocationException.hpp"

LocationException::LocationException(const std::string& Search) : Search(Search) { }

LocationException::~LocationException() _NOEXCEPT
{
	
}

const char* LocationException::what () const throw () {
	return "Failed to find matching location!";
}
