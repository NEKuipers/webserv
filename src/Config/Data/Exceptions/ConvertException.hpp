#ifndef CONVERTEXCEPTION_HPP
#define CONVERTEXCEPTION_HPP

#include <exception>
#include <string>

struct ConvertException : public std::exception
{
	const std::string From;
	const std::string To;
	const std::string Reason;

	ConvertException(const std::string& From, const std::string& To, const std::string& Reason);

	virtual ~ConvertException() _NOEXCEPT;

	const char* what () const throw ();

	friend std::ostream& operator<<(std::ostream& Stream, const ConvertException& Exception);
};

#endif
