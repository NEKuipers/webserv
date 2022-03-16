#include "LocationException.hpp"

LocationException::LocationException(const std::string& Search) : Search(Search) { }

LocationException::~LocationException() _GLIBCXX_USE_NOEXCEPT
{

}

const char* LocationException::what () const throw () {
	return "Failed to find matching location!";
}

std::ostream& operator<<(std::ostream& Stream, const LocationException& Exception)
{
	Stream << "Failed to find location matching " << Exception.Search << "!";
	return Stream;
}
