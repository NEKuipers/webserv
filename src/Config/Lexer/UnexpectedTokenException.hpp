#ifndef UNEXPECTEDTOKENEXCEPTION_HPP
#define UNEXPECTEDTOKENEXCEPTION_HPP

#include "Lexer.hpp"
#include <exception>
#include <ostream>

struct UnexpectedTokenException : public std::exception
{
	Token Expected;
	Token Actual;
	std::string error_location;
	UnexpectedTokenException(Token Expected, Token Actual, std::string error_location);
	~UnexpectedTokenException() throw();

	const char* what () const throw ();

	friend std::ostream& operator<<(std::ostream& Stream, const UnexpectedTokenException& Exception);
};

#endif
