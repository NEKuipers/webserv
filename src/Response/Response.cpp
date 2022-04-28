#include "CGIResponse.hpp"
#include "CGIRunner.hpp"
#include "ClientSocket.hpp"
#include "ConfigCgiResponse.hpp"
#include "ConfigDeleteResponse.hpp"
#include "ConfigDirectoryResponse.hpp"
#include "ConfigErrorResponse.hpp"
#include "ConfigFileResponse.hpp"
#include "ConfigRedirectResponse.hpp"
#include "ConfigUploadFileResponse.hpp"
#include "PathUtils.hpp"
#include "Response.hpp"
#include "SimpleResponse.hpp"
#include "ToString.hpp"
#include <cassert>	// linux assert()
#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <map>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 1024


static std::map<int, std::string> g_response_code_to_reason_phrase;

void Response::InitStatusCodes()
{
	assert(g_response_code_to_reason_phrase.size() == 0);

	g_response_code_to_reason_phrase[100] = "Continue"; //-
	g_response_code_to_reason_phrase[101] = "Switching Protocols"; //-
	g_response_code_to_reason_phrase[200] = "OK"; //-
	g_response_code_to_reason_phrase[201] = "Created"; //-
	g_response_code_to_reason_phrase[202] = "Accepted";  //-
	g_response_code_to_reason_phrase[203] = "Non-Authoritative Information";  //-
	g_response_code_to_reason_phrase[204] = "No Content"; //-
	g_response_code_to_reason_phrase[205] = "Reset Content";  //-
	g_response_code_to_reason_phrase[206] = "Partial Content";  //-
	g_response_code_to_reason_phrase[300] = "Multiple Choices";  //-
	g_response_code_to_reason_phrase[301] = "Moved Permanently"; //-
	g_response_code_to_reason_phrase[302] = "Found";  //-
	g_response_code_to_reason_phrase[303] = "See Other";  //-
	g_response_code_to_reason_phrase[304] = "Not Modified";  //-
	g_response_code_to_reason_phrase[305] = "Use Proxy";  //-
	g_response_code_to_reason_phrase[307] = "Temporary Redirect";  //-
	g_response_code_to_reason_phrase[400] = "Bad Request"; //-
	g_response_code_to_reason_phrase[401] = "Unauthorized";  //-
	g_response_code_to_reason_phrase[402] = "Payment Required";  //-
	g_response_code_to_reason_phrase[403] = "Forbidden";  //-
	g_response_code_to_reason_phrase[404] = "Not Found"; //-
	g_response_code_to_reason_phrase[405] = "Method Not Allowed"; //-
	g_response_code_to_reason_phrase[406] = "Not Acceptable";  //-
	g_response_code_to_reason_phrase[407] = "Proxy Authentication Required";  //-
	g_response_code_to_reason_phrase[408] = "Request Timeout";  //-
	g_response_code_to_reason_phrase[409] = "Conflict";  //-
	g_response_code_to_reason_phrase[410] = "Gone";  //-
	g_response_code_to_reason_phrase[411] = "Length Required";  //-
	g_response_code_to_reason_phrase[412] = "Precondition Failed";  //-
	g_response_code_to_reason_phrase[413] = "Payload Too Large"; //-
	g_response_code_to_reason_phrase[414] = "URI Too Long";  //-
	g_response_code_to_reason_phrase[415] = "Unsupported Media Type";  //-
	g_response_code_to_reason_phrase[416] = "Range Not Satisfiable";  //-
	g_response_code_to_reason_phrase[417] = "Expectation Failed";  //-
	g_response_code_to_reason_phrase[426] = "Upgrade Required";  //-
	g_response_code_to_reason_phrase[500] = "Internal Server Error";
	g_response_code_to_reason_phrase[501] = "Not Implemented";  //-
	g_response_code_to_reason_phrase[502] = "Bad Gateway";  //-
	g_response_code_to_reason_phrase[503] = "Service Unavailable";  //-
	g_response_code_to_reason_phrase[504] = "Gateway Timeout";  //-
	g_response_code_to_reason_phrase[505] = "HTTP Version Not Supported";  //-
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

	if (status_code == 405) {
		headers_string += "Allow:";
		for (std::vector<std::string>::const_iterator it = conf_response->GetErrorReasons().GetAllowedMethods().begin(); it != conf_response->GetErrorReasons().GetAllowedMethods().end(); it++)
			headers_string += " " + *it;
		headers_string += "\r\n";
	}

	if (ConfigRedirectResponse* RedirectResponsePtr = dynamic_cast<ConfigRedirectResponse*>(conf_response))
		headers_string += "Location: " + RedirectResponsePtr->GetNewPath() + "\r\n";
	else if (status_code == 201)
	 	headers_string += "Location: " + request.get_request_line().path + "\r\n";		
	return (headers_string);
}

std::string	Response::create_status_line(int status_code)
{
	std::string http_version = "HTTP/1.1";
	std::string status_line = http_version + " " + to_string(status_code) + " " + get_reason_phrase(status_code) + "\r\n";
	return status_line;
}

int	Response::delete_method(const std::string& fullpath)
{
	// Note: Due to only being called via the config, the file should always be there, and *should* not be a directory 
	if (PathUtils::pathType(fullpath) == PathUtils::FILE
	 && unlink(fullpath.c_str()) == 0)
		return 204; //204 No Content is the correct status code for a successful delete request (yes, really)
	return 404;
}

int	Response::create_method(const std::string& fullpath, const std::string& contents)
{
	// Note: status code here is 200 if file is appended and 201 if file is created
	int return_value = PathUtils::pathType(fullpath);
	return_value = (return_value == PathUtils::FILE) ? 200 : 201;
	std::ofstream file;
	file.open(fullpath, std::ofstream::out | std::ofstream::app);
	if (file.bad())
		return 403;	// Odd
	file << contents;
	return return_value;
}

std::string	Response::create_directory_listing(const std::string& directory_path, const std::string& target)
{
	std::string dir_listing = "<!DOCTYPE html><head><title>Listing of " + target+"</title></head> \
								<body><h1>Listing of <span>";
	dir_listing += target;
	dir_listing +="</span></h1><ul>";
	DIR *dr;
	std::string listing = "";
	struct dirent *en;
	dr = opendir(directory_path.c_str());
	while ((en = readdir(dr)) != 0)
		listing += "<li><a href=\"" + target + std::string(en->d_name) +  "\">" + std::string(en->d_name) + "</a></li>";
	closedir(dr);
	dir_listing += listing + "</ul></body></html>";
	return (dir_listing);
}

std::string	Response::make_error_page(int status_code)
{
	return "<!DOCTYPE html><html><p style=\"text-align:center;font-size:200%;\"><a href=\"/\">Webserv</a><br><br><b>" + to_string(status_code) + " " + get_reason_phrase(status_code) +"</b><br><p style=\"line-height: 5000em;text-align:right\"><b>h</b></div></p></html>";
}

Response	*Response::generate_response(ConfigResponse *conf_response, Request &request)
{
	assert(g_response_code_to_reason_phrase.size() != 0);

	//std::cout << request << std::endl;
	(void)request;
	std::string body = "";
	std::string content_type = "text/html";
	int status_code = 400;

	std::cout << conf_response->GetErrorReasons() << std::endl;
	conf_response->Print(std::cout);

	if (dynamic_cast<ConfigErrorResponse*>(conf_response))
	{
		status_code = 404;
		if (conf_response->GetErrorReasons().GetWasWrongMethod())
			status_code = 405;
		else if (conf_response->GetErrorReasons().GetWasBodyTooBig())
			status_code = 413;
	}
	else if (request.get_body().size() > 0 && request.get_request_line().method != "POST")
		status_code = 400;
	else if (ConfigDirectoryResponse* DirectoryResponsePtr = dynamic_cast<ConfigDirectoryResponse*>(conf_response))
	{
		status_code = 200;
		body = create_directory_listing(DirectoryResponsePtr->GetDirectory(), request.get_request_line().path);
	}
	else if (ConfigRedirectResponse* RedirectResponsePtr = dynamic_cast<ConfigRedirectResponse*>(conf_response))
		status_code = RedirectResponsePtr->GetCode();
	else if (ConfigUploadFileResponse* UploadFileResponsePtr = dynamic_cast<ConfigUploadFileResponse*>(conf_response))
		status_code = create_method(UploadFileResponsePtr->GetFileName(), request.get_body());
	else if (ConfigDeleteResponse* DeleteResponsePtr = dynamic_cast<ConfigDeleteResponse*>(conf_response))
		status_code = delete_method(DeleteResponsePtr->GetDeleteFile());
	else if (ConfigFileResponse* FileResponsePtr = dynamic_cast<ConfigFileResponse*>(conf_response))
	{
		status_code = 200;
		content_type = FileResponsePtr->GetContentType();
		body = to_string(FileResponsePtr->GetStream().rdbuf());
		// if allowed methods contains GET, 404, otherwise: wrong method?
		bool HasGet = std::find(conf_response->GetErrorReasons().GetAllowedMethods().begin(), conf_response->GetErrorReasons().GetAllowedMethods().end(), "GET") != conf_response->GetErrorReasons().GetAllowedMethods().end();

		if (conf_response->GetErrorReasons().GetWasErrorPage())
			status_code = HasGet ? 404 : 405;
		
	}
	else if (ConfigCgiResponse* CgiResponsePtr = dynamic_cast<ConfigCgiResponse*>(conf_response))
	{
		std::map<std::string, std::string> map;
		CgiResponsePtr->MakeEnvMap(map, request);
		return new CGIResponse(new CGIRunner(CgiResponsePtr->GetCgiFile(), map));
	}
	else
	{
		status_code = 404;
		body = "<!DOCTYPE html><html><p style=\"text-align:center;font-size:200%;\"><a href=\"/\">Webserv</a><br><br><b>Unknown response type:<br>";
		body.append(to_string(*conf_response));
		body.append("</b><br><p style=\"line-height: 5000em;text-align:right\"><b>h</b></div></p></html>");
	}

	std::string response_string = create_status_line(status_code);
	if (status_code >= 400 && status_code < 500 && body == "")
	{
		body = make_error_page(status_code);
		content_type = "text/html";
	}
	if (body != "")
	{
		response_string += "Content-Type: " + content_type + "\r\n";
		response_string += "Content-Length: " + to_string(body.length()) + "\r\n";
	}
	response_string += create_headers(conf_response, request, status_code);
	response_string += "\r\n" + body;
	//std::cerr << response_string << std::endl;
	//std::cout << "Response: " << to_string(*conf_response) << std::endl;
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
