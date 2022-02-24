class ConfigLine;	// For include loops

#ifndef CONFIGLINE_HPP
#define CONFIGLINE_HPP

#include "Parser.hpp"
#include "ConfigBlock.hpp"
#include <vector>
#include <string>
#include <ostream>

// ConfigLine: A list of strings on the line + a possible config block
class ConfigLine {
	public:
		ConfigLine();
		ConfigLine(Parser& Parser);
		ConfigLine(const ConfigLine& From);

		~ConfigLine();

		ConfigLine& operator = (const ConfigLine& From);

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigLine& Line);
		ConfigBlock* GetBlock() const;
	private:
		// Class variables
		std::vector<std::string> Arguments;
		ConfigBlock* Block;
		// Class functions
};

#endif
