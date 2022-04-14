#include "Response.hpp"
#include "ClientSocket.hpp"
#include "FileResponse.hpp"
#include "ErrorResponse.hpp"
#include "ToString.hpp"
#include "Request.hpp"
#include <cstdlib>

std::map<int, std::string> g_response_code_to_reason_phrase;

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


Response::~Response(){}

Response::Response(const Response &src)
{
    *this = src;
}

Response::Response(ConfigResponse *conf_response)
{
	std::string headers = "";
	std::string body = "";
	std::string content_type = "";
	std::stringstream scode;
	int status_code = 400;

	if (FileResponse* FileResponsePtr = dynamic_cast<FileResponse*>(conf_response))
	{
		status_code = 200;
		content_type = FileResponsePtr->GetContentType();
		body = to_string(FileResponsePtr->GetStream().rdbuf());
	}
	else if (!conf_response || dynamic_cast<ErrorResponse*>(conf_response))
	{
		status_code = 404;
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
	scode << status_code;
	std::string http_version = "HTTP/1.1"; //TODO figure out if this is always the case??
	response_string = http_version + " " + scode.str() + " " + g_response_code_to_reason_phrase[status_code] + "\r\n";
	response_string += headers;//TODO add headers here
	response_string += "Content-Type: " + content_type + "\r\n";
	response_string += "Content-Length: " + to_string(body.length()) + "\r\n";	
	time_t now = time(0);
	char *datetime = ctime(&now);
	response_string += datetime;
	response_string += "\r\nServer: webserv\r\n";
	response_string += "\r\n";
	response_string += body;
}

std::string			Response::get_response_string()
{
	return response_string;
}
