#include "ConfigBlock.hpp"
#include "PrefixStreambuf.hpp"


ConfigBlock::ConfigBlock() : Lines() {}
ConfigBlock::ConfigBlock(Parser& Parser) : Lines() {
	Parser.Read(OpenParen);

	while (Parser.Peek() == Word)
		Lines.push_back(ConfigLine(Parser));

	Parser.Read(CloseParen);
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
