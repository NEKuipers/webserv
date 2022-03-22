#ifndef METHODEXCEPTION_HPP
#define METHODEXCEPTION_HPP

#include <exception>
#include <string>
#include <ostream>

struct MethodException : public std::exception
{
	const std::string Type;
	const std::string Method;

	MethodException(const std::string& Type, const std::string& Method);

	virtual ~MethodException() _NOEXCEPT;

	const char* what () const throw ();

	friend std::ostream& operator<<(std::ostream& Stream, const MethodException& Exception);
};

#endif
