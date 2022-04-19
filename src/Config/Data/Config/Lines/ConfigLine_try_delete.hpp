class ConfigLine_try_delete;	// For include loops

#ifndef CONFIGLINE_TRY_DELETE_HPP
#define CONFIGLINE_TRY_DELETE_HPP

#include <ostream>
#include "ConfigBase.hpp"
#include <vector>
#include "ConfigErrorReasons.hpp"

class ConfigLine_try_delete : public ConfigBase {
	public: 
		ConfigLine_try_delete();
		ConfigLine_try_delete(const ConfigLine_try_delete& From);
		ConfigLine_try_delete(const std::vector<std::string>& Files, const ConfigurationState& Configuration);

		virtual  ~ConfigLine_try_delete();

		ConfigLine_try_delete& operator = (const ConfigLine_try_delete& From);

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigLine_try_delete& ConfigLine_try_delete);
		void Print(std::ostream& Stream) const;
		
		static ConfigLine_try_delete* TryParse(const ConfigLine& Line, const ConfigurationState& Configuration);
	protected:
		virtual ConfigResponse* GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const;
		virtual bool WouldHaveResponded(const ConfigRequest& Request) const;
	private:
		std::vector<std::string> Files;
		// Class variables
		
		// Class functions
		
};

#endif
