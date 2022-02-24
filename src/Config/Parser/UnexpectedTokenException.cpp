#include "UnexpectedTokenException.hpp"


UnexpectedTokenException::UnexpectedTokenException(Token Expected, Token Actual) : Expected(Expected), Actual(Actual) { }

const char* UnexpectedTokenException::what () const throw () {
	return "Unexpected Token Exception!";	// I'd love to have it tell me what it expected and what is actually got, but this is C++98, we dont do string formatting here!
}
