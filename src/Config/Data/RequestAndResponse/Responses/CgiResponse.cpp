#include "CgiResponse.hpp"

CgiResponse::CgiResponse(const std::string& CgiFile) : CgiFile(CgiFile)
{
	
}
CgiResponse::~CgiResponse()
{
	
}

const std::string& CgiResponse::GetCgiFile() const { return CgiFile; }

std::ostream& operator<<(std::ostream& Stream, const CgiResponse& CgiResponse)
{
	CgiResponse.Print(Stream);
	return Stream;
}

void CgiResponse::Print(std::ostream& PrintStream) const
{
	PrintStream << "CgiResponse " << CgiFile;
}