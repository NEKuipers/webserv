class ConfigListBase;	// For include loops

#ifndef CONFIGLISTBASE_HPP
#define CONFIGLISTBASE_HPP

#include <ostream>
#include "ConfigBase.hpp"

enum EnterResult
{
	EnterResult_No,
	EnterResult_Enter,
	EnterResult_EnterAndError
};

class ConfigListBase : public ConfigBase {
	public: 
		ConfigListBase();
		ConfigListBase(const ConfigurationState& Configuration);

		virtual ~ConfigListBase();

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigListBase& ConfigListBase);
		virtual void Print(std::ostream& Stream) const;
	protected:

		ConfigResponse* GetBaseResponse(const ConfigRequest& Request) const;

		virtual EnterResult Enters(const ConfigRequest& Request) const = 0;

		std::vector<ConfigBase*> Children;
	private:
		// We dont do copying
		ConfigListBase(const ConfigListBase& From);
		ConfigListBase& operator = (const ConfigListBase& From);
};

#endif
