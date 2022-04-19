class ConfigLine_upload;	// For include loops

#ifndef CONFIGLINE_UPLOAD_HPP
#define CONFIGLINE_UPLOAD_HPP

#include <ostream>
#include "ConfigBase.hpp"
#include "ConfigErrorReasons.hpp"

class ConfigLine_upload : public ConfigBase {
	public: 
		ConfigLine_upload();
		ConfigLine_upload(const ConfigLine_upload& From);
		ConfigLine_upload(const std::string& TargetLocation, const ConfigurationState& Configuration);

		virtual ~ConfigLine_upload();

		ConfigLine_upload& operator = (const ConfigLine_upload& From);

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigLine_upload& ConfigLine_upload);
		virtual void Print(std::ostream& Stream) const;
		
		static ConfigLine_upload* TryParse(const ConfigLine& Line, const ConfigurationState& Configuration);
	protected:
		virtual ConfigResponse* GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const;
		virtual bool WouldHaveResponded(const ConfigRequest& Request) const;
	private:
		// Class variables
		std::string TargetLocation;
		
		// Class functions
		
};

#endif
