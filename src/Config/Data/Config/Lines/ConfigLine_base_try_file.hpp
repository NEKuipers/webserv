#ifndef CONFIGLINE_BASE_TRY_FILE_HPP
#define CONFIGLINE_BASE_TRY_FILE_HPP

#include <ostream>
#include "ConfigBase.hpp"
#include <vector>
#include "ConfigErrorReasons.hpp"

class ConfigLine_base_try_file : public ConfigBase {
	public: 
		ConfigLine_base_try_file();
		ConfigLine_base_try_file(const ConfigLine_base_try_file& From);
		ConfigLine_base_try_file(const std::vector<std::string>& Files, const std::string& ConfigLineName, const std::string* const* AcceptedMethods, const ConfigurationState& Configuration);	// I hate that you can't call virtual functions in the constructor, now i have 2 extra arguments!

		virtual  ~ConfigLine_base_try_file();

		ConfigLine_base_try_file& operator = (const ConfigLine_base_try_file& From);

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigLine_base_try_file& ConfigLine_base_try_file);
		virtual void Print(std::ostream& Stream) const;		
	protected:
		virtual ConfigResponse* GetResponseForFile(const ConfigRequest& Request, const std::string& FullPath, const ConfigErrorReasons &ErrorReasons) const = 0;
		virtual bool AcceptsPartialPath() const;

		virtual ConfigResponse* GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const;
		virtual bool WouldHaveResponded(const ConfigRequest& Request) const;
	private:
		std::vector<std::string> Files;
		// Class variables
		
		// Class functions
		
};

#endif
