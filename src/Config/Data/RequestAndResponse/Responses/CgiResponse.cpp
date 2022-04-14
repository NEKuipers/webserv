#include "CgiResponse.hpp"
#include "Request.hpp"
#include "ToString.hpp"

CgiResponse::CgiResponse(const std::string& CgiFile, const ConfigCombinedResponse& CombinedResponse) : ConfigResponse(CombinedResponse), CgiFile(CgiFile)
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

bool CgiResponse::RequiresBody() const
{
	return true;
}

void CgiResponse::MakeEnvMap(std::map<std::string, std::string>& Map, const Request& Request)
{
	Map["SERVER_SOFTWARE"] = "webserv";
	Map["SERVER_NAME"] = "127.0.0.1";	// TODO: Double check
	Map["GATEWAY_INTERFACE"] = "CGI/1.1";	// TODO: Double check

	Map["SERVER_PROTOCOL"] = "HTTP/1.1";
	Map["SERVER_PORT"] = to_string(ntohs(RequestPort));
	Map["REQUEST_METHOD"] = Request.get_request_line().method;
	if (PATH_INFO != "")
	{
		Map["PATH_INFO"] = PATH_INFO;
		Map["PATH_TRANSLATED"] = CgiFile + PATH_INFO;	// TODO: Double check
	}
	Map["SCRIPT_NAME"] = CgiFile;
	Map["QUERY_STRING"] = QUERY_STRING;
	//Map["REMOTE_HOST"] = ?
	Map["REMOTE_ADDR"] = "127.0.0.1";	// TODO: Get adress from Request

	const std::string& ContentType = Request.get_header_value("Content-Type");
	if (ContentType != "")
	{
		Map["CONTENT_TYPE"] = ContentType;
		Map["CONTENT_LENGTH"] = Request.get_header_value("Content-Length");
	}

	const std::map<std::string, std::string>& HeaderFields = Request.get_header_fields();
	for (std::map<std::string, std::string>::const_iterator it = HeaderFields.begin(); it != HeaderFields.end(); it++)
	{
		std::string Upper = it->first;
		for (size_t i = 0; i < Upper.length(); i++)
			Upper.at(i) = std::toupper(Upper.at(i));
		Map["HTTP_" + Upper] = it->second;
	}
}