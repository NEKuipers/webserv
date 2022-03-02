#ifndef UNEXPECTEDTOKENEXCEPTION_HPP
#define UNEXPECTEDTOKENEXCEPTION_HPP

#include "Lexer.hpp"
#include <exception>

struct UnexpectedTokenException : public std::exception
{
	Token Expected;
	Token Actual;

	UnexpectedTokenException(Token Expected, Token Actual);

	const char* what () const throw ();
};

#endif
