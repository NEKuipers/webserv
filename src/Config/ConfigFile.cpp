#include "ConfigFile.hpp"
#include <fstream>

ConfigFile::ConfigFile() {}
ConfigFile::ConfigFile(const std::string& FilePath) : ConfigLines()
{
	std::fstream Stream(FilePath.c_str());

	Parser Parser(Stream);

	while (Parser.Peek() != EndOfFile)
		ConfigLines.push_back(ConfigLine(Parser));
}
ConfigFile::ConfigFile(const ConfigFile& From)
{
	this->operator=(From);
}

ConfigFile::~ConfigFile()
{
	// TODO: Implement destructor
}

ConfigFile& ConfigFile::operator = (const ConfigFile& From)
{
	ConfigLines = From.ConfigLines;

	// return the existing object so we can chain this operator
	return *this;
}

std::ostream& operator<<(std::ostream& Stream, const ConfigFile& File)
{
	for (size_t i = 0; i < File.ConfigLines.size(); i++)
	{
		if (i != 0)
			Stream << "\n";
		Stream << File.ConfigLines[i];
	}
	return Stream;
}