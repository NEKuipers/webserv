class ConfigListBase;	// For include loops

#ifndef CONFIGLISTBASE_HPP
#define CONFIGLISTBASE_HPP

#include <ostream>
#include "ConfigBase.hpp"
#include "ConfigErrorReasons.hpp"

enum EnterResult
{
	EnterResult_No,
	EnterResult_Enter,
	EnterResult_EnterAndError
};


class ConfigListBase : public ConfigBase {
	public:
		typedef ConfigBase* (*TryParseLineFunc)(const ConfigLine &Line, const ConfigurationState &Configuration);
		static TryParseLineFunc BaseLines[];	// NOTE: For every different context i would want another array

		ConfigListBase();
		ConfigListBase(const ConfigurationState& Configuration);

		virtual ~ConfigListBase();

		// Public functions
		friend std::ostream& operator<<(std::ostream& Stream, const ConfigListBase& ConfigListBase);
		virtual void Print(std::ostream& Stream) const;

		const std::vector<ConfigBase*>& GetChildren();
	protected:
		void ReadBlock(const std::string& CreateClass, const TryParseLineFunc* NullTerminatedParseFuncs, const std::vector<ConfigLine>& Lines);
		virtual bool EatLine(const ConfigLine& Line);

		virtual ConfigResponse* GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const;
		virtual bool ChecksConfiguration() const;

		virtual EnterResult Enters(const ConfigRequest& Request) const = 0;

		bool AddToChildren(ConfigBase* ConfigBase);
		std::vector<ConfigBase*> Children;
	private:
		// We dont do copying
		ConfigListBase(const ConfigListBase& From);
		ConfigListBase& operator = (const ConfigListBase& From);
};

#endif
