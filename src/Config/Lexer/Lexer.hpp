class Lexer;	// For include loops

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

// TODO: Nice error messages using Stream.tellg(); zoals [filename]:line:columm
class Lexer {
	public:
		Lexer(std::istream& Stream);
		~Lexer();

		// Public functions
		Token Peek();
		std::string Read(Token Expected);

		static std::string TokenToString(Token Token);
	private:
		// No default constructors or copying
		Lexer();
		Lexer& operator = (const Lexer& From);
		Lexer(const Lexer& From);

		// Class variables
		std::istream& Stream;
		Token NextToken;
		std::string NextString;

		std::string UnparsedString;

		// Class functions
		void Step();
};

#endif
