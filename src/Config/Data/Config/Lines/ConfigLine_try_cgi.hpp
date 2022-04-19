class ConfigLine_try_cgi;	// For include loops

#ifndef CONFIGLINE_TRY_CGI_HPP
#define CONFIGLINE_TRY_CGI_HPP

#include <ostream>
#include <vector>
#include "ConfigLine_base_try_file.hpp"

class ConfigLine_try_cgi : public ConfigLine_base_try_file {
	public: 
		ConfigLine_try_cgi();
		ConfigLine_try_cgi(const ConfigLine_try_cgi& From);
		ConfigLine_try_cgi(const std::vector<std::string>& cgis, const ConfigurationState& Configuration);

		virtual  ~ConfigLine_try_cgi();

		ConfigLine_try_cgi& operator = (const ConfigLine_try_cgi& From);

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigLine_try_cgi& ConfigLine_try_cgi);
		virtual void Print(std::ostream& Stream) const;
		
		static ConfigLine_try_cgi* TryParse(const ConfigLine& Line, const ConfigurationState& Configuration);
	protected:
		virtual ConfigResponse* GetResponseForFile(const ConfigRequest& Request, const std::string& FullPath, const ConfigErrorReasons &ErrorReasons) const;
		virtual bool AcceptsPartialPath() const;
};

#endif
