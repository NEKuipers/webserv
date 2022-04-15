#include "ConfigErrorResponse.hpp"

ConfigErrorResponse::ConfigErrorResponse(const ConfigErrorReasons& ErrorReasons) : ConfigResponse(ErrorReasons) {}
ConfigErrorResponse::~ConfigErrorResponse() { }

std::ostream& operator<<(std::ostream& Stream, const ConfigErrorResponse& ConfigErrorResponse)
{
	(void)ConfigErrorResponse;
	Stream << "Error Response";
	return Stream;
}

void ConfigErrorResponse::Print(std::ostream& Stream) const
{
	Stream << *this;
}