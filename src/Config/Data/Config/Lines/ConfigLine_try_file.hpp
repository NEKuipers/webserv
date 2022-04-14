class ConfigLine_try_file;	// For include loops

#ifndef CONFIGLINE_TRY_FILE_HPP
#define CONFIGLINE_TRY_FILE_HPP

#include <ostream>
#include "ConfigBase.hpp"
#include <vector>
#include "ConfigCombinedResponse.hpp"

class ConfigLine_try_file : public ConfigBase {
	public: 
		ConfigLine_try_file();
		ConfigLine_try_file(const ConfigLine_try_file& From);
		ConfigLine_try_file(const std::vector<std::string>& Files, const ConfigurationState& Configuration);

		virtual  ~ConfigLine_try_file();

		ConfigLine_try_file& operator = (const ConfigLine_try_file& From);

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigLine_try_file& ConfigLine_try_file);
		void Print(std::ostream& Stream) const;
		
		static ConfigLine_try_file* TryParse(const ConfigLine& Line, const ConfigurationState& Configuration);
	protected:
		virtual ConfigResponse* GetBaseResponse(const ConfigRequest& Request, ConfigCombinedResponse& CombinedResponse) const;
		virtual void AddCombinedResponseIfNoResponse(const ConfigRequest& Request, ConfigCombinedResponse& CombinedResponse) const;
	private:
		std::vector<std::string> Files;
		// Class variables
		
		// Class functions
		
};

#endif
