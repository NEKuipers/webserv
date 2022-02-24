#include "Parser.hpp"
#include "UnexpectedTokenException.hpp"

Parser::Parser(std::istream& Stream) : Stream(Stream)
{
	Step();
}

Parser::~Parser() { }


Token Parser::Peek()
{
	return NextToken;
}

std::string Parser::Read(Token Expected)
{
	if (NextToken != Expected)
		throw UnexpectedTokenException(Expected, NextToken);

	std::string Value = NextString;
	Step();
	return Value;
}

void Parser::Step()
{
	// TODO: Parse "Hello;" as [Word, "Hello"] [Stop, ";"]

	if (!(Stream >> NextString && NextString.length() > 0))
	{
		NextToken = EndOfFile;
		return;
	}

	switch (NextString.at(0))
	{
		case '{': NextToken = OpenParen; break;
		case '}': NextToken = CloseParen; break;
		case ';': NextToken = Stop; break;
		default : NextToken = Word; break;
	}
}
