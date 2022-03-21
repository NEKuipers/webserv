#include "UnexpectedTokenException.hpp"


UnexpectedTokenException::UnexpectedTokenException(Token Expected, Token Actual) : Expected(Expected), Actual(Actual) { }

const char* UnexpectedTokenException::what () const throw () {
	return "Unexpected Token Exception!";	// I'd love to have it tell me what it expected and what is actually got, but this is C++98, we dont do string formatting here!
}

std::ostream& operator<<(std::ostream& Stream, const UnexpectedTokenException& Exception)
{
	// TODO: Log location of token
	Stream << "Unexpected token: " << Lexer::TokenToString(Exception.Actual) << ", Expected: " << Lexer::TokenToString(Exception.Expected) << "!" << std::endl;
	return Stream;
}
