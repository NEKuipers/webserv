class Parser;	// For include loops

#ifndef PARSER_HPP
#define PARSER_HPP

#include <istream>
#include <string>

enum Token
{
	Word,
	OpenParen,
	CloseParen,
	Stop,
	EndOfFile
};

class Parser {
	public:
		Parser(std::istream& Stream);

		~Parser();

		// Public functions
		Token Peek();
		std::string Read(Token Expected);
	private:
		// No default constructors or copying
		Parser();
		Parser& operator = (const Parser& From);
		Parser(const Parser& From);

		// Class variables
		std::istream& Stream;
		Token NextToken;
		std::string NextString;

		std::string UnparsedString;

		// Class functions
		void Step();
};

#endif
