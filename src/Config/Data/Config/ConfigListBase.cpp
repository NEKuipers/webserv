#include "ConfigListBase.hpp"
#include "PrefixStreambuf.hpp"

#include "ConfigLine_try_file.hpp"
#include "ConfigLine_try_cgi.hpp"
#include "ConfigLine_internal_redirect.hpp"
#include "ConfigLine_server.hpp"
#include "ConfigLine_location.hpp"
#include "ConfigLine_index.hpp"
#include "ConfigLine_try_delete.hpp"
#include "ConfigLine_upload.hpp"
#include "ConfigLine_redirect.hpp"

#include "ConvertException.hpp"

// Yeah yeah, function pointer casting, should be fine, args are the same and return value is a derrived class pointer casted to the base class pointer
ConfigListBase::TryParseLineFunc ConfigListBase::BaseLines[] = {
	(ConfigListBase::TryParseLineFunc) ConfigLine_try_file			::TryParse,
	(ConfigListBase::TryParseLineFunc) ConfigLine_internal_redirect	::TryParse,
	(ConfigListBase::TryParseLineFunc) ConfigLine_server			::TryParse,
	(ConfigListBase::TryParseLineFunc) ConfigLine_location			::TryParse,
	(ConfigListBase::TryParseLineFunc) ConfigLine_index				::TryParse,
	(ConfigListBase::TryParseLineFunc) ConfigLine_try_cgi			::TryParse,
	(ConfigListBase::TryParseLineFunc) ConfigLine_try_delete		::TryParse,
	(ConfigListBase::TryParseLineFunc) ConfigLine_upload			::TryParse,
	(ConfigListBase::TryParseLineFunc) ConfigLine_redirect			::TryParse,
	NULL
};

ConfigListBase::ConfigListBase() : ConfigBase() { }
ConfigListBase::ConfigListBase(const ConfigurationState& Configuration) : ConfigBase(Configuration) { }

ConfigListBase::~ConfigListBase()
{
	for (std::vector<ConfigBase*>::const_iterator It = Children.begin(); It != Children.end(); It++)
		delete *It;
}

const std::vector<ConfigBase*>& ConfigListBase::GetChildren() { return Children; }

ConfigResponse* ConfigListBase::GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const
{
	bool ErrorIfNoResult;

	EnterResult Result = Enters(Request);
	switch (Result)
	{
		case EnterResult_No:
			return NULL;
		case EnterResult_Enter:
			ErrorIfNoResult = false;
			break;
		case EnterResult_EnterAndError:
			ErrorIfNoResult = true;
			break;
		default:
			throw "Unknown EnterResult was returned!";
	}

	for (std::vector<ConfigBase*>::const_iterator It = Children.begin(); It != Children.end();) // It++)
	{
		const ConfigBase* Child = *It;

		ConfigResponse* Response = Child->GetIteratorResponse(It, Children.end(), Request, ErrorReasons);
		if (Response)
			return Response;
	}

	if (ErrorIfNoResult)
		return Configuration.Error(Request, ErrorReasons);
	return NULL;
}

bool ConfigListBase::AddToChildren(ConfigBase* ConfigBase)
{
	if (!ConfigBase)
		return false;
	Children.push_back(ConfigBase);
	return true;
}

void ConfigListBase::ReadBlock(const std::string& CreateClass, const TryParseLineFunc* NullTerminatedParseFuncs, const std::vector<ConfigLine>& Lines)
{
	for (std::vector<ConfigLine>::const_iterator It = Lines.begin(); It != Lines.end(); It++)
	{
		const ConfigLine& Line = *It;
		if (Configuration.EatLine(Line) || EatLine(Line))
			goto AteLine;

		for (size_t i = 0; NullTerminatedParseFuncs[i]; i++)
			if (AddToChildren(NullTerminatedParseFuncs[i](Line, Configuration)))
				goto AteLine;	// Using goto because i had to break out of a loop, and also skip over the throw after the loop

		throw ConvertException("ConfigLine", CreateClass, "Could not determine the meaning of line: '" + Line.GetArguments()[0] + "' in '" + CreateClass + "' context");
		AteLine:;
	}
}

bool ConfigListBase::EatLine(const ConfigLine& Line)
{
	(void)Line;
	return false;
}

bool ConfigListBase::ChecksConfiguration() const
{
	return false;
}

std::ostream& operator<<(std::ostream& Stream, const ConfigListBase& ConfigListBase)
{
	ConfigListBase.Print(Stream);
	return Stream;
}

void ConfigListBase::Print(std::ostream& Stream) const
{
	Stream << "{\n";
	PrefixStreambuf Prefix(Stream, "  ");
	for (std::vector<ConfigBase*>::const_iterator It = Children.begin(); It != Children.end(); It++)
	{
		(*It)->Print(Stream);
		Stream << "\n";
	}
	Prefix.End();
	Stream << "}";
}
