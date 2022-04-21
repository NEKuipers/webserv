class ConfigLine_internal_redirect;	// For include loops

#ifndef CONFIGLINE_INTERNAL_REDIRECT_HPP
#define CONFIGLINE_INTERNAL_REDIRECT_HPP

#include <ostream>
#include "ConfigBase.hpp"
#include "ConfigErrorReasons.hpp"

class ConfigLine_internal_redirect : public ConfigBase {
	public: 
		ConfigLine_internal_redirect();
		ConfigLine_internal_redirect(const ConfigLine_internal_redirect& From);
		ConfigLine_internal_redirect(const std::string& NewPath, const ConfigurationState& Configuration);

		virtual ~ConfigLine_internal_redirect();

		ConfigLine_internal_redirect& operator = (const ConfigLine_internal_redirect& From);

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigLine_internal_redirect& ConfigLine_redirect);
		virtual void Print(std::ostream& Stream) const;
		
		static ConfigLine_internal_redirect* TryParse(const ConfigLine& Line, const ConfigurationState& Configuration);
	protected:
		virtual ConfigResponse* GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const;

	private:
		// Class variables
		std::string NewPath;
		
		// Class functions
		
};

#endif
