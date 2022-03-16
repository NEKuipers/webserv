#ifndef LOCATIONEXCEPTION_HPP
#define LOCATIONEXCEPTION_HPP

#include <exception>
#include <string>
#include <ostream>

struct LocationException : public std::exception
{
	const std::string Search;

	LocationException(const std::string& Search);

	virtual ~LocationException() _NOEXCEPT;

	const char* what () const throw ();

	friend std::ostream& operator<<(std::ostream& Stream, const LocationException& Exception);
};

#endif