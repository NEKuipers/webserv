#include "ConfigCgiResponse.hpp"
#include "Request.hpp"
#include "ToString.hpp"

ConfigCgiResponse::ConfigCgiResponse(const std::string& CgiFile, const std::string& PathInfo, const std::string& ScriptName, const ConfigErrorReasons& ErrorReasons) : ConfigResponse(ErrorReasons), CgiFile(CgiFile), PathInfo(PathInfo), ScriptName(ScriptName)
{
	
}
ConfigCgiResponse::~ConfigCgiResponse()
{
	
}

const std::string& ConfigCgiResponse::GetCgiFile() const { return CgiFile; }

std::ostream& operator<<(std::ostream& Stream, const ConfigCgiResponse& ConfigCgiResponse)
{
	ConfigCgiResponse.Print(Stream);
	return Stream;
}

void ConfigCgiResponse::Print(std::ostream& PrintStream) const
{
	PrintStream << "ConfigCgiResponse " << CgiFile;
}

bool ConfigCgiResponse::RequiresBody() const
{
	return true;
}

void ConfigCgiResponse::MakeEnvMap(std::map<std::string, std::string>& Map, const Request& Request)
{
	Map["SERVER_SOFTWARE"] = "webserv";
	Map["SERVER_NAME"] = Request.get_header_value("Host");	
	Map["GATEWAY_INTERFACE"] = "CGI/1.1";

	Map["SERVER_PROTOCOL"] = "HTTP/1.1";
	Map["SERVER_PORT"] = to_string(ntohs(RequestPort));
	Map["REQUEST_METHOD"] = Request.get_request_line().method;
	if (PathInfo != "")
	{
		Map["PATH_INFO"] = PathInfo;
		Map["PATH_TRANSLATED"] = CgiFile + PathInfo;
	}
	Map["SCRIPT_NAME"] = ScriptName;
	Map["QUERY_STRING"] = QUERY_STRING;
	//Map["REMOTE_HOST"] = ?
	Map["REMOTE_ADDR"] = "127.0.0.1"; // TODO: Get adress from ClientSocket

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