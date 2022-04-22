#include "UnexpectedTokenException.hpp"


UnexpectedTokenException::UnexpectedTokenException(Token Expected, Token Actual, std::string error_location) : Expected(Expected), Actual(Actual), error_location(error_location){ }

const char* UnexpectedTokenException::what () const throw () {
	return "Unexpected Token Exception!";	// I'd love to have it tell me what it expected and what is actually got, but this is C++98, we dont do string formatting here!
}

std::ostream& operator<<(std::ostream& Stream, const UnexpectedTokenException& Exception)
{
	// TODO: Log location of token
	Stream << "Unexpected token at position "<<Exception.error_location << ": " << Lexer::TokenToString(Exception.Actual) << ", Expected: " << Lexer::TokenToString(Exception.Expected) << "!" << std::endl;
	return Stream;
}

UnexpectedTokenException::~UnexpectedTokenException() throw() {}
