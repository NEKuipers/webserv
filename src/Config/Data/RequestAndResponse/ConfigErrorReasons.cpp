#include "ConfigErrorReasons.hpp"

ConfigErrorReasons::ConfigErrorReasons() : AllowedMethods(), AllowsAllMethods(false), WasWrongMethod(false), WasBodyTooBig(false) {}
ConfigErrorReasons::ConfigErrorReasons(const ConfigErrorReasons& From) : AllowedMethods(), AllowsAllMethods(false), WasWrongMethod(false), WasBodyTooBig(false)
{
	this->operator=(From);
}

ConfigErrorReasons::~ConfigErrorReasons()
{
}

ConfigErrorReasons& ConfigErrorReasons::operator = (const ConfigErrorReasons& From)
{
	AllowedMethods = From.AllowedMethods;
	WasWrongMethod = From.WasWrongMethod;
	WasBodyTooBig = From.WasBodyTooBig;
	WasErrorPage = From.WasErrorPage;
	AllowsAllMethods = From.AllowsAllMethods;

	// return the existing object so we can chain this operator
	return *this;
}

std::ostream& operator<<(std::ostream& Stream, const ConfigErrorReasons& ConfigErrorReasons)
{
	Stream << "ConfigErrorReasons:\n\tAllowed methods:";
	for (std::vector<std::string>::const_iterator it = ConfigErrorReasons.AllowedMethods.begin(); it != ConfigErrorReasons.AllowedMethods.end(); it++)
		Stream << " " << *it;
	if (ConfigErrorReasons.WasWrongMethod) Stream << "\n\tWrong method!";
	if (ConfigErrorReasons.WasBodyTooBig) Stream << "\n\tBody too big!";
	if (ConfigErrorReasons.WasErrorPage) Stream << "\n\tError page returned!";
	return Stream;
}

const std::vector<std::string>& ConfigErrorReasons::GetAllowedMethods() const { return AllowedMethods; }
void ConfigErrorReasons::AddAllowedMethod(const std::string& Method)
{
	if (GetWasErrorPage()) return;
	
	for (std::vector<std::string>::const_iterator it = AllowedMethods.begin(); it != AllowedMethods.end(); it++)
		if (*it == Method)
			return;
	AllowedMethods.push_back(Method);
}

void ConfigErrorReasons::AddAllowedMethods(const std::vector<std::string>& Methods)
{
	if (GetWasErrorPage()) return;
	
	if (Methods.size() == 0 )	// Its a hack
		AllowsAllMethods = true;
	
	for (std::vector<std::string>::const_iterator it = Methods.begin(); it != Methods.end(); it++)
		AddAllowedMethod(*it);
}

bool ConfigErrorReasons::GetAllowsAllMethods() const { return AllowsAllMethods; }

void ConfigErrorReasons::Err_WrongMethod() { if (GetWasErrorPage()) return; WasWrongMethod = true; }
void ConfigErrorReasons::Err_BodyTooBig() { if (GetWasErrorPage()) return; WasBodyTooBig = true; }
void ConfigErrorReasons::Err_ErrorPage() { if (GetWasErrorPage()) return; WasErrorPage = true; }

bool ConfigErrorReasons::GetWasWrongMethod() const { return WasWrongMethod; }
bool ConfigErrorReasons::GetWasBodyTooBig() const { return WasBodyTooBig; }
bool ConfigErrorReasons::GetWasErrorPage() const { return WasErrorPage; }