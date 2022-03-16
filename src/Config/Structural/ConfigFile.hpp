class ConfigFile;	// For include loops

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "ConfigLine.hpp"
#include <vector>

#include <iostream>

// ConfigFile: Has a list of ConfigLines that contains all the actual data, but adds functions to read out the data easily
class ConfigFile {
	public:
		ConfigFile();
		ConfigFile(const std::string& FilePath);
		ConfigFile(const ConfigFile& From);

		~ConfigFile();

		ConfigFile& operator = (const ConfigFile& From);

		// Public functions
		const std::vector<ConfigLine>& GetConfigLines() const;

		friend std::ostream& operator<<(std::ostream& Stream, const ConfigFile& File);
	private:
		// Class variables
		std::vector<ConfigLine> ConfigLines;
		// Class functions

};

#endif
