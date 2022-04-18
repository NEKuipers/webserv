#include "Response.hpp"
#include "ClientSocket.hpp"
#include "ConfigFileResponse.hpp"
#include "ConfigErrorResponse.hpp"
#include "ConfigCgiResponse.hpp"
#include "CGIResponse.hpp"
#include "SimpleResponse.hpp"
#include "PathUtils.hpp"
#include <map>
#include "CGIRunner.hpp"
#include "ToString.hpp"
#include <cstdlib>
#include <sys/time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cassert>	// linux assert()

#define BUFFER_SIZE 1024


static std::map<int, std::string> g_response_code_to_reason_phrase;

void Response::InitStatusCodes()
{
	assert(g_response_code_to_reason_phrase.size() == 0);

	g_response_code_to_reason_phrase[100] = "Continue";
	g_response_code_to_reason_phrase[101] = "Switching Protocols";
	g_response_code_to_reason_phrase[200] = "OK";
	g_response_code_to_reason_phrase[201] = "Created"; // For POST method: use stat() to find out if target resource already exists, if not, open(with O_CREAT flag) and return 201
	g_response_code_to_reason_phrase[202] = "Accepted";
	g_response_code_to_reason_phrase[203] = "Non-Authoritative Information";
	g_response_code_to_reason_phrase[204] = "No Content";
	g_response_code_to_reason_phrase[205] = "Reset Content";
	g_response_code_to_reason_phrase[206] = "Partial Content";
	g_response_code_to_reason_phrase[300] = "Multiple Choices";
	g_response_code_to_reason_phrase[301] = "Moved Permanently";
	g_response_code_to_reason_phrase[302] = "Found";
	g_response_code_to_reason_phrase[303] = "See Other";
	g_response_code_to_reason_phrase[304] = "Not Modified";
	g_response_code_to_reason_phrase[305] = "Use Proxy";
	g_response_code_to_reason_phrase[307] = "Temporary Redirect";
	g_response_code_to_reason_phrase[400] = "Bad Request";
	g_response_code_to_reason_phrase[401] = "Unauthorized";
	g_response_code_to_reason_phrase[402] = "Payment Required";
	g_response_code_to_reason_phrase[403] = "Forbidden";
	g_response_code_to_reason_phrase[404] = "Not Found";
	g_response_code_to_reason_phrase[405] = "Method Not Allowed"; // implemented
	g_response_code_to_reason_phrase[406] = "Not Acceptable";
	g_response_code_to_reason_phrase[407] = "Proxy Authentication Required";
	g_response_code_to_reason_phrase[408] = "Request Timeout";
	g_response_code_to_reason_phrase[409] = "Conflict";
	g_response_code_to_reason_phrase[410] = "Gone";
	g_response_code_to_reason_phrase[411] = "Length Required";
	g_response_code_to_reason_phrase[412] = "Precondition Failed";
	g_response_code_to_reason_phrase[413] = "Payload Too Large";
	g_response_code_to_reason_phrase[414] = "URI Too Long";
	g_response_code_to_reason_phrase[415] = "Unsupported Media Type";
	g_response_code_to_reason_phrase[416] = "Range Not Satisfiable";
	g_response_code_to_reason_phrase[417] = "Expectation Failed";
	g_response_code_to_reason_phrase[426] = "Upgrade Required";
	g_response_code_to_reason_phrase[500] = "Internal Server Error";
	g_response_code_to_reason_phrase[501] = "Not Implemented";
	g_response_code_to_reason_phrase[502] = "Bad Gateway";
	g_response_code_to_reason_phrase[503] = "Service Unavailable";
	g_response_code_to_reason_phrase[504] = "Gateway Timeout";
	g_response_code_to_reason_phrase[505] = "HTTP Version Not Supported";
}

static std::string format_timestamp(time_t timestamp)
{
	char buffer[33];
	struct tm *ts;
	size_t last;

	ts   = localtime(&timestamp);
	last = strftime(buffer, 32, "%a, %d %b %Y %T GMT", ts);
	buffer[last] = '\0';
	return (std::string(buffer));
}

static std::string get_date_header()
{
	struct timeval now;
	struct timezone tz;

	gettimeofday(&now, &tz);
	return (format_timestamp(now.tv_sec + tz.tz_minuteswest * 60));
}


std::string Response::create_headers(ConfigResponse *conf_response, Request &request, int status_code)
{
	(void)request;

	std::string headers_string = "";
	headers_string += "Date: " + get_date_header() + "\r\n";
	headers_string += "Server: webserv\r\n";
	headers_string += "Allow:";
	if (conf_response)
		for (std::vector<std::string>::const_iterator it = conf_response->GetErrorReasons().GetAllowedMethods().begin(); it != conf_response->GetErrorReasons().GetAllowedMethods().end(); it++)
			headers_string += " " + *it;
	headers_string += "\r\n";
	if (status_code == 201)
	{
		headers_string += "Location: ";
		//dynamic_cast<PostResponse*>(conf_response)->created_uri;
	}
	return (headers_string);
}

std::string	Response::create_status_line(int status_code)
{
	std::string http_version = "HTTP/1.1";
	std::string status_line = http_version + " " + to_string(status_code) + " " + get_reason_phrase(status_code) + "\r\n";
	return status_line;
}

int	Response::delete_method(Request &request)
{
	std::string prefix = "http/default";
	std::string fullpath = prefix + request.get_request_line().target;
	//if (PathUtils::pathType(request.get_request_line().target) == PathUtils::FILE)
	if (PathUtils::pathType(fullpath) == PathUtils::FILE)
	{
		unlink(fullpath.c_str());
		return 200;
	}
	return 404;
}

Response	*Response::generate_response(ConfigResponse *conf_response, Request &request)
{
	assert(g_response_code_to_reason_phrase.size() != 0);

	(void)request;
	std::string body = "";
	std::string content_type = "";
	int status_code = 400;
	std::string cgi_response = "";

	//if (conf_response)
	//	std::cout << conf_response->GetErrorReasons() << std::endl;
	if (request.get_request_line().method == "DELETE")
	{
		status_code = delete_method(request);
		std::string response_string = create_status_line(status_code);
		response_string += create_headers(conf_response, request, status_code);
		// std::cerr<<status_code<<"\n";
		return new SimpleResponse(response_string);
	}
	if (ConfigFileResponse* FileResponsePtr = dynamic_cast<ConfigFileResponse*>(conf_response))
	{
		status_code = 200;
		content_type = FileResponsePtr->GetContentType();
		body = to_string(FileResponsePtr->GetStream().rdbuf());
	}
	else if (ConfigCgiResponse* CgiResponsePtr = dynamic_cast<ConfigCgiResponse*>(conf_response))
	{
		std::map<std::string, std::string> map;
		CgiResponsePtr->MakeEnvMap(map, request);
		return new CGIResponse(new CGIRunner(CgiResponsePtr->GetCgiFile(), map));
	}
	else if (!conf_response || dynamic_cast<ConfigErrorResponse*>(conf_response))
	{
		status_code = 404;
		if (conf_response)
		{
			if (conf_response->GetErrorReasons().GetWasWrongMethod())
				status_code = 405;
			else if (conf_response->GetErrorReasons().GetWasBodyTooBig())
				status_code = 413;
		}

		content_type = "text/html";
		body = "<!DOCTYPE html><html><p style=\"text-align:center;font-size:200%;\"><a href=\"/\">Webserv</a><br><br><b>Default error page<br>You seem to have made a invalid request!</b><br><p style=\"line-height: 5000em;text-align:right\"><b>h</b></div></p></html>";
	}
	else
	{
		status_code = 404;
		content_type = "text/html";

		body = "<!DOCTYPE html><html><p style=\"text-align:center;font-size:200%;\"><a href=\"/\">Webserv</a><br><br><b>Unknown response type:<br>";
		body.append(to_string(*conf_response));
		body.append("</b><br><p style=\"line-height: 5000em;text-align:right\"><b>h</b></div></p></html>");
	}


	std::string response_string = create_status_line(status_code);
	if (cgi_response == "")
	{
		response_string += "Content-Type: " + content_type + "\r\n";
		response_string += "Content-Length: " + to_string(body.length()) + "\r\n";
	}
	response_string += create_headers(conf_response, request, status_code);//TODO add headers here
	if (cgi_response != "")
		response_string += cgi_response;
	else
		response_string += "\r\n" + body;

	if (conf_response)
		std::cout << "Response: " << to_string(*conf_response) << std::endl;
	return new SimpleResponse(response_string);
}

Response::Response()
{

}

Response::~Response(){}

Response::Response(const Response &src)
{
    *this = src;
}

std::string			Response::get_reason_phrase(int status_code)
{
	std::map<int, std::string>::const_iterator it = g_response_code_to_reason_phrase.find(status_code);
	if (it == g_response_code_to_reason_phrase.end())
		return ("ERROR");
	return (it->second);
}
