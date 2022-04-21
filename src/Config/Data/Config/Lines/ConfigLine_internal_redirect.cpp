#include "ConfigLine_internal_redirect.hpp"
#include "ConvertException.hpp"

#include "ToString.hpp"

ConfigLine_internal_redirect::ConfigLine_internal_redirect() {}
ConfigLine_internal_redirect::ConfigLine_internal_redirect(const ConfigLine_internal_redirect& From)
{
	this->operator=(From);
}
ConfigLine_internal_redirect::ConfigLine_internal_redirect(const std::string& NewPath, const ConfigurationState& Configuration) : ConfigBase(Configuration), NewPath(NewPath)
{

}

ConfigLine_internal_redirect::~ConfigLine_internal_redirect()
{

}

ConfigLine_internal_redirect& ConfigLine_internal_redirect::operator = (const ConfigLine_internal_redirect& From)
{
	static_cast<ConfigBase*>(this)->operator=(From);
	NewPath = From.NewPath;

	// return the existing object so we can chain this operator
	return *this;
}

std::ostream& operator<<(std::ostream& Stream, const ConfigLine_internal_redirect& ConfigLine_internal_redirect)
{
	ConfigLine_internal_redirect.Print(Stream);
	return Stream;
}

void ConfigLine_internal_redirect::Print(std::ostream& Stream) const
{
	Stream << "Redirect " << NewPath;
}

ConfigLine_internal_redirect* ConfigLine_internal_redirect::TryParse(const ConfigLine& Line, const ConfigurationState& Configuration)
{
	std::vector<std::string> Args = Line.GetArguments();
	if (Args.at(0) != "internal_redirect")
		return NULL;

	if (Args.size() != 2)
		throw ConvertException("ConfigLine", "ConfigLine_internal_redirect", "Bad argument count! Expected 2, Got " + to_string(Args.size()));

	return new ConfigLine_internal_redirect(Args.at(1), Configuration);
}

ConfigResponse* ConfigLine_internal_redirect::GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const
{
	return Configuration.Redirect(Request, NewPath, ErrorReasons);
}