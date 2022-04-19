#include "ConfigLine_redirect.hpp"
#include "ConvertException.hpp"

#include "ToString.hpp"

ConfigLine_redirect::ConfigLine_redirect() {}
ConfigLine_redirect::ConfigLine_redirect(const ConfigLine_redirect& From)
{
	this->operator=(From);
}
ConfigLine_redirect::ConfigLine_redirect(const std::string& NewPath, const ConfigurationState& Configuration) : ConfigBase(Configuration), NewPath(NewPath)
{

}

ConfigLine_redirect::~ConfigLine_redirect()
{

}

ConfigLine_redirect& ConfigLine_redirect::operator = (const ConfigLine_redirect& From)
{
	static_cast<ConfigBase*>(this)->operator=(From);
	NewPath = From.NewPath;

	// return the existing object so we can chain this operator
	return *this;
}

std::ostream& operator<<(std::ostream& Stream, const ConfigLine_redirect& ConfigLine_redirect)
{
	ConfigLine_redirect.Print(Stream);
	return Stream;
}

void ConfigLine_redirect::Print(std::ostream& Stream) const
{
	Stream << "Redirect " << NewPath;
}

ConfigLine_redirect* ConfigLine_redirect::TryParse(const ConfigLine& Line, const ConfigurationState& Configuration)
{
	std::vector<std::string> Args = Line.GetArguments();
	if (Args.at(0) != "redirect")
		return NULL;

	if (Args.size() != 2)
		throw ConvertException("ConfigLine", "ConfigLine_redirect", "Bad argument count! Expected 2, Got " + to_string(Args.size()));

	return new ConfigLine_redirect(Args.at(1), Configuration);
}

ConfigResponse* ConfigLine_redirect::GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const
{
	return Configuration.Redirect(Request, NewPath, ErrorReasons);
}