#include "ConfigErrorReasons.hpp"

ConfigErrorReasons::ConfigErrorReasons() : AllowedMethods(), WasWrongMethod(false), WasBodyTooBig(false) {}
ConfigErrorReasons::ConfigErrorReasons(const ConfigErrorReasons& From) : AllowedMethods(), WasWrongMethod(false), WasBodyTooBig(false)
{
	this->operator=(From);
}

ConfigErrorReasons::~ConfigErrorReasons()
{
	// TODO: Implement destructor
}

ConfigErrorReasons& ConfigErrorReasons::operator = (const ConfigErrorReasons& From)
{
	AllowedMethods = From.AllowedMethods;
	WasWrongMethod = From.WasWrongMethod;
	WasBodyTooBig = From.WasBodyTooBig;

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
	return Stream;
}

const std::vector<std::string>& ConfigErrorReasons::GetAllowedMethods() const { return AllowedMethods; }
void ConfigErrorReasons::AddAllowedMethod(const std::string& Method)
{
	for (std::vector<std::string>::const_iterator it = AllowedMethods.begin(); it != AllowedMethods.end(); it++)
		if (*it == Method)
			return;
	AllowedMethods.push_back(Method);
}

void ConfigErrorReasons::AddAllowedMethods(const std::vector<std::string>& Methods)
{
	for (std::vector<std::string>::const_iterator it = Methods.begin(); it != Methods.end(); it++)
		AddAllowedMethod(*it);
}

void ConfigErrorReasons::Err_WrongMethod() { WasWrongMethod = true; }
void ConfigErrorReasons::Err_BodyTooBig() { WasBodyTooBig = true; }

bool ConfigErrorReasons::GetWasWrongMethod() const { return WasWrongMethod; }
bool ConfigErrorReasons::GetWasBodyTooBig() const { return WasBodyTooBig; }