#ifndef CONVERTEXCEPTION_HPP
#define CONVERTEXCEPTION_HPP

#include <exception>
#include <string>

struct ConvertException : public std::exception
{
	const std::string Message;

	ConvertException(const std::string& Message);

	virtual ~ConvertException() _GLIBCXX_USE_NOEXCEPT;

	const char* what () const throw ();
};

#endif
