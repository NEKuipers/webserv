#include "ConfigBlock.hpp"
#include "PrefixStreambuf.hpp"


ConfigBlock::ConfigBlock() : Lines() {}
ConfigBlock::ConfigBlock(Lexer& Lexer) : Lines() {
	Lexer.Read(OpenParen);

	while (Lexer.Peek() == Word)
		Lines.push_back(ConfigLine(Lexer));

	Lexer.Read(CloseParen);
}
ConfigBlock::ConfigBlock(const ConfigBlock& From) : Lines()
{
	this->operator=(From);
}

ConfigBlock::~ConfigBlock()
{

}

ConfigBlock& ConfigBlock::operator = (const ConfigBlock& From)
{
	Lines = From.GetLines();

	// return the existing object so we can chain this operator
	return *this;
}

const std::vector<ConfigLine>& ConfigBlock::GetLines() const { return Lines; }

std::ostream& operator<<(std::ostream& Stream, const ConfigBlock& Block)
{
	Stream << "{";

	PrefixStreambuf Indent(Stream, "  ");

	for (size_t i = 0; i < Block.Lines.size(); i++)
		Stream << "\n" << Block.Lines[i];

	Indent.End();

	Stream << "\n}";
	return Stream;
}
