#include "Response.hpp"
#include "ClientSocket.hpp"
#include "ConfigFileResponse.hpp"
#include "ConfigErrorResponse.hpp"
#include "ConfigCgiResponse.hpp"
#include "CGI_Response.hpp"
#include <map>
#include "CGIRunner.hpp"
#include "ToString.hpp"
#include <cstdlib>
#include <sys/time.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024


static std::map<int, std::string> g_response_code_to_reason_phrase;

void Response::InitContentTypes()
{
	assert(g_response_code_to_reason_phrase.size() == 0);

	g_response_code_to_reason_phrase[100] = "Continue";
	g_response_code_to_reason_phrase[101] = "Switching Protocols";
	g_response_code_to_reason_phrase[200] = "OK";
	g_response_code_to_reason_phrase[201] = "Created";
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
	g_response_code_to_reason_phrase[405] = "Method Not Allowed";
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

Response	*Response::generate_response(ConfigResponse *conf_response, Request &request)
{
	assert(g_response_code_to_reason_phrase.size() != 0);
	
	(void)request;
	std::string body = "";
	std::string content_type = "";
	int status_code = 400;
	std::string cgi_response = "";

	if (conf_response)
		std::cout << conf_response->GetErrorReasons() << std::endl;

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
		CGIRunner runner(CgiResponsePtr->GetCgiFile(), map);
		
		runner.QueuePartialBodyForWrite(request.get_body());	// NOTE: Considering this is CGI, it does not need to be started AFTER the whole body has been read, it can be done in sections, so the CGI program can be started earlier
		
		while (!runner.Write()) {  }			// TODO: Non-Blocking read loop via select
		while (!runner.Read(cgi_response)) { }	// TODO: Non-Blocking read loop via select

		//std::cout << "cgi_response = " << cgi_response << std::endl;
		status_code = 200;
		// TODO: Magic stuff
		// NOTE: cgi_response is [headers]\n\r[body] without the http line
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

	std::string http_version = "HTTP/1.1";
	std::string response_string = http_version + " " + to_string(status_code) + " " + get_reason_phrase(status_code) + "\r\n";
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
	if (cgi_response != "")
		return new CGIResponse();
	return new Response(response_string);
}

Response::Response(const std::string& response_string) : response_string(response_string)
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

bool			Response::get_response_string(std::string &response_string) const
{
	response_string = this->response_string;
	return true;
}
