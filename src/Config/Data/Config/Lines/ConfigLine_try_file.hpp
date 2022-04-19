class ConfigLine_try_file;	// For include loops

#ifndef CONFIGLINE_TRY_FILE_HPP
#define CONFIGLINE_TRY_FILE_HPP

#include <ostream>
#include "ConfigBase.hpp"
#include <vector>
#include "ConfigErrorReasons.hpp"
#include "ConfigLine_base_try_file.hpp"

class ConfigLine_try_file : public ConfigLine_base_try_file {
	public: 
		ConfigLine_try_file();
		ConfigLine_try_file(const ConfigLine_try_file& From);
		ConfigLine_try_file(const std::vector<std::string>& Files, const ConfigurationState& Configuration);

		virtual  ~ConfigLine_try_file();

		ConfigLine_try_file& operator = (const ConfigLine_try_file& From);

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigLine_try_file& ConfigLine_try_file);
		virtual void Print(std::ostream& Stream) const;
		
		static ConfigLine_try_file* TryParse(const ConfigLine& Line, const ConfigurationState& Configuration);
	protected:
		virtual ConfigResponse* GetResponseForFile(const ConfigRequest& Request, const std::string& FullPath, const ConfigErrorReasons &ErrorReasons) const;
};

#endif
