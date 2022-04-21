#include "ConfigLine_redirect.hpp"
#include "ConvertException.hpp"
#include "ConfigRedirectResponse.hpp"

#include "ToString.hpp"

ConfigLine_redirect::ConfigLine_redirect() {}
ConfigLine_redirect::ConfigLine_redirect(const ConfigLine_redirect& From)
{
	this->operator=(From);
}
ConfigLine_redirect::ConfigLine_redirect(int Code, const std::string& NewPath, const ConfigurationState& Configuration) : ConfigBase(Configuration), Code(Code), NewPath(NewPath)
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

	if (Args.size() != 3)
		throw ConvertException("ConfigLine", "ConfigLine_redirect", "Bad argument count! Expected 3, Got " + to_string(Args.size()));

	char* End;
	unsigned long ULCode = std::strtoul(Args.at(1).c_str(), &End, 10);
	int Code = ULCode;

	if ((unsigned long)Code != ULCode)
		throw ConvertException("ConfigLine", "ConfigLine_redirect code", "Code too big");
	else if (End != &Args.at(1).c_str()[Args.at(1).length()])
		throw ConvertException("ConfigLine", "ConfigLine_redirect code", "Code contained more than just a number");

	return new ConfigLine_redirect(Code, Args.at(2), Configuration);
}

ConfigResponse* ConfigLine_redirect::GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const
{
	bool MustValidate = false;
	ErrorReasons.AddAllowedMethods(Configuration.AcceptedMethods);
	return new ConfigRedirectResponse(Code, Configuration.InterperetEnvVariableUserVariables(NewPath, Request, MustValidate), ErrorReasons);
}