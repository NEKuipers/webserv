#include "ConfigLine.hpp"
#include "ConfigBlock.hpp"

ConfigLine::ConfigLine() : Arguments(), Block() {}
ConfigLine::ConfigLine(Parser& Parser) : Arguments(), Block() {
	std::string Current;

	while (Parser.Peek() == Word)
		Arguments.push_back(Parser.Read(Word));

	if (Parser.Peek() == OpenParen)
		Block = new ConfigBlock(Parser);
	else
		Parser.Read(Stop);
}
ConfigLine::ConfigLine(const ConfigLine& From) : Arguments(), Block()
{
	this->operator=(From);
}

ConfigLine::~ConfigLine()
{
	delete Block;
}

ConfigLine& ConfigLine::operator = (const ConfigLine& From)
{
	Arguments = From.Arguments;
	delete Block;
	Block = From.Block ? new ConfigBlock(*From.Block) : NULL;

	// return the existing object so we can chain this operator
	return *this;
}


ConfigBlock* ConfigLine::GetBlock() const { return Block; }

std::ostream& operator<<(std::ostream& Stream, const ConfigLine& Line)
{
	for (size_t i = 0; i < Line.Arguments.size(); i++)
	{
		if (i != 0)
			Stream << " ";
		Stream << Line.Arguments[i];
	}
	if (Line.Block != NULL)
		Stream << " " << *Line.Block;
	else
		Stream << ";";
	return Stream;
}
