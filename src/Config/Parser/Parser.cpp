#include "Parser.hpp"
#include "UnexpectedTokenException.hpp"

#include <iostream>

Parser::Parser(std::istream& Stream) : Stream(Stream), UnparsedString("")
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
	// [UnparsedString]: If we only returned a part of the string last time, this is the leftover
	if (UnparsedString.length() == 0)
	{
		// We need the next section, we have nothing left over from the last step
		if (!(Stream >> UnparsedString && UnparsedString.length() > 0))
		{
			NextToken = EndOfFile;
			return;
		}
	}

	size_t NumChars;
	switch (UnparsedString.at(0))
	{
		case '{': NextToken = OpenParen;	NumChars = 1; break;
		case '}': NextToken = CloseParen;	NumChars = 1; break;
		case ';': NextToken = Stop;			NumChars = 1; break;
		default :
			NextToken = Word;
			NumChars = UnparsedString.find_first_of("{};");	// if no matches found, returns -1, due to it being unsigned, its the max number
			break;
	}

	// Cut [NumChars] from [UnparsedString] into [NextString]
	NextString = UnparsedString.substr(0, NumChars);
	UnparsedString = NumChars < UnparsedString.length() ? UnparsedString.substr(NumChars) : "";
}
