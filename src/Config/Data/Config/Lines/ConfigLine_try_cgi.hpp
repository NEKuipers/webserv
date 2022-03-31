class ConfigLine_try_cgi;	// For include loops

#ifndef CONFIGLINE_TRY_CGI_HPP
#define CONFIGLINE_TRY_CGI_HPP

#include <ostream>
#include "ConfigBase.hpp"
#include <vector>

class ConfigLine_try_cgi : public ConfigBase {
	public: 
		ConfigLine_try_cgi();
		ConfigLine_try_cgi(const ConfigLine_try_cgi& From);
		ConfigLine_try_cgi(const std::vector<std::string>& cgis, const ConfigurationState& Configuration);

		virtual  ~ConfigLine_try_cgi();

		ConfigLine_try_cgi& operator = (const ConfigLine_try_cgi& From);

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigLine_try_cgi& ConfigLine_try_cgi);
		void Print(std::ostream& Stream) const;
		
		static ConfigLine_try_cgi* TryParse(const ConfigLine& Line, const ConfigurationState& Configuration);
	protected:
		virtual ConfigResponse* GetBaseResponse(const ConfigRequest& Request) const;
		
	private:
		std::vector<std::string> cgis;
		// Class variables
		
		// Class functions
		
};

#endif
