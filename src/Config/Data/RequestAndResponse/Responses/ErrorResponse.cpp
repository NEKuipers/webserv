#include "ErrorResponse.hpp"

ErrorResponse::ErrorResponse(const ConfigCombinedResponse& CombinedResponse) : ConfigResponse(CombinedResponse) {}
ErrorResponse::~ErrorResponse() { }

std::ostream& operator<<(std::ostream& Stream, const ErrorResponse& ErrorResponse)
{
	(void)ErrorResponse;
	Stream << "Error Response";
	return Stream;
}

void ErrorResponse::Print(std::ostream& Stream) const
{
	Stream << *this;
}