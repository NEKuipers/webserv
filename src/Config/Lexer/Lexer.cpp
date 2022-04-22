#include "Lexer.hpp"
#include "UnexpectedTokenException.hpp"

#include <iostream>
#include "ToString.hpp"

Lexer::Lexer(std::istream& Stream) : Stream(Stream), UnparsedString("")
{
	Step();
}

Lexer::~Lexer() { }


Token Lexer::Peek()
{
	return NextToken;
}

std::string Lexer::Read(Token Expected)
{
	int pos = Stream.tellg();
	std::string error_location = to_string(pos);
	if (NextToken != Expected)
		throw UnexpectedTokenException(Expected, NextToken, error_location);
	std::string Value = NextString;
	Step();
	return Value;
}

void Lexer::Step()
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
		case '#':
			// This is a comment, read untill next \n, clear unparsed string, and return the next step
			std::getline(Stream, UnparsedString);
			UnparsedString = "";
			return Step();
		default :
			NextToken = Word;
			NumChars = UnparsedString.find_first_of("{};#");	// if no matches found, returns -1, due to it being unsigned, its the max number
			break;
	}

	// Cut [NumChars] from [UnparsedString] into [NextString]
	NextString = UnparsedString.substr(0, NumChars);
	UnparsedString = NumChars < UnparsedString.length() ? UnparsedString.substr(NumChars) : "";
}

std::string Lexer::TokenToString(Token Token)
{
	switch (Token)
	{
		case EndOfFile: return "EOF";
		case OpenParen: return "OpenParen";
		case CloseParen: return "CloseParen";
		case Stop: return "Stop";
		case Word: return "Word";
		default: return "Unknown token: " + to_string(Token);
	}
}
