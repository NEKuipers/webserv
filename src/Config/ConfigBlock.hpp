class ConfigBlock;	// For include loops

#ifndef CONFIGBLOCK_HPP
#define CONFIGBLOCK_HPP

#include <vector>
#include "ConfigLine.hpp"
#include "Parser.hpp"

// ConfigBlock: A block containing multiple lines
class ConfigBlock {
	public:
		ConfigBlock();
		ConfigBlock(Parser& Parser);
		ConfigBlock(const ConfigBlock& From);

		~ConfigBlock();

		ConfigBlock& operator = (const ConfigBlock& From);

		// Public functions
		const std::vector<ConfigLine>& GetLines() const;

		friend std::ostream& operator<<(std::ostream& Stream, const ConfigBlock& Block);
	private:
		// Class variables
		std::vector<ConfigLine> Lines;
		// Class functions

};

#endif
