class ConfigLine_index;	// For include loops

#ifndef CONFIGLINE_INDEX_HPP
#define CONFIGLINE_INDEX_HPP

#include <ostream>
#include "ConfigListBase.hpp"

class ConfigLine_index : public ConfigListBase {
	public: 
		ConfigLine_index();
		ConfigLine_index(const ConfigBlock& Block, const ConfigurationState &Configuration);

		virtual ~ConfigLine_index();

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigLine_index& ConfigLine_index);
		void Print(std::ostream& Stream) const;

		static ConfigLine_index* TryParse(const ConfigLine& Line, const ConfigurationState& Configuration);
	protected:
		virtual EnterResult Enters(const ConfigRequest& Request) const;
	private:
		ConfigLine_index(const ConfigLine_index& From);
		ConfigLine_index& operator = (const ConfigLine_index& From);

		// Class variables

		// TODO: Option to have it list all the files in this directory when no block is given

		// Class functions
};

#endif
