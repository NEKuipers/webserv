#include "LocationException.hpp"

LocationException::LocationException(const std::string& Search) : Search(Search) { }

const char* LocationException::what () const throw () {
	return "Failed to find matching location!";
}
