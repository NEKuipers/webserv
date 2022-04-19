class ConfigLine_redirect;	// For include loops

#ifndef CONFIGLINE_REDIRECT_HPP
#define CONFIGLINE_REDIRECT_HPP

#include <ostream>
#include "ConfigBase.hpp"
#include "ConfigErrorReasons.hpp"

class ConfigLine_redirect : public ConfigBase {
	public: 
		ConfigLine_redirect();
		ConfigLine_redirect(const ConfigLine_redirect& From);
		ConfigLine_redirect(const std::string& NewUri, const ConfigurationState& Configuration);

		virtual ~ConfigLine_redirect();

		ConfigLine_redirect& operator = (const ConfigLine_redirect& From);

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigLine_redirect& ConfigLine_redirect);
		virtual void Print(std::ostream& Stream) const;
		
		static ConfigLine_redirect* TryParse(const ConfigLine& Line, const ConfigurationState& Configuration);
	protected:
		virtual ConfigResponse* GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const;

	private:
		// Class variables
		std::string NewUri;
		
		// Class functions
		
};

#endif
