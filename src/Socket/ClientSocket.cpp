#include "ClientSocket.hpp"
#include "FileResponse.hpp"
#include "ToString.hpp"
#include "Request.hpp"
#define BUFFER_SIZE 3000
#include <unistd.h>
#include <map>
#include "ReadFromSocketException.hpp"

// Constructor
ClientSocket::ClientSocket(struct sockaddr_in address, int sock) : SimpleSocket(address, sock), headers_complete(false), written_size(0), response(NULL)
{

}

ClientSocket::~ClientSocket()
{
	delete response;
}

	// Step 1: Read next line
	// Step 2: If first line, call parse_requestline on request, otherwise, call parse_single_header_field, if it returns true, call validate_request() & return HEADER_COMPLETE
	// Step 3: If header is complete, wait till buffer has [content-length] of bytes, once it is, call set_request_body and return BODY_COMPLETE



void 	ClientSocket::createResponse()
{
	std::string Status;
	// TODO: Response headers
	std::string Headers = "";
	std::string ContentType = "";
	std::string Body;


	if (FileResponse* FileResponsePtr = dynamic_cast<FileResponse*>(response))
	{
		Status = "200 OK";
		ContentType = FileResponsePtr->GetContentType();
		Body = to_string(FileResponsePtr->GetStream().rdbuf());
	}
	else
	{
		if (response)
		{
			Status = "200 OK";
			ContentType = "text/html";

			Body = "<!DOCTYPE html><head><title>Webserv Testpage</title></head><body><p>Hello World!\n";
			Body.append("Unknown response type: " + to_string(*response));
			Body.append("<p></body></html>");
		}
		else
		{
			// I guess this is a default error page or something i dunno
			Status = "404 OK";
			ContentType = "text/html";
			Body = "<!DOCTYPE html><head><title>Webserv ErrorPage</title></head><body><p>You have made a invalid request!<p></body></html>";
		}
	}

	// Debugging infos
	time_t now = time(0);
	char *datetime = ctime(&now);
	Body.append(datetime);

	if (response)
		Body.append("Response type: " + to_string(*response));

	to_write = "HTTP/1.1 " + Status + "\r\n";
	to_write += Headers;
	to_write += "[Content-Type]: " + ContentType + "\r\n";
	to_write += "[Content-Length]: " + to_string(Body.length()) + "\r\n";
	to_write += "\r\n";	// End of headers
	to_write += Body;
}

Request					ClientSocket::get_request()
{
	return request;
}

bool					ClientSocket::send()
{
	written_size += ::send(get_sock(), to_write.c_str()+written_size, to_write.size()-written_size, 0);
	return written_size >= ssize_t(to_write.size()); //TODO if send returns -1, throw exception and remove client
}

void					ClientSocket::read()
{
	char	read_buffer[BUFFER_SIZE];
	int ret = recv(get_sock(), read_buffer, BUFFER_SIZE, MSG_DONTWAIT);
	if (ret < 0)
	{
		throw ReadFromSocketException();
	}
	buffer += std::string(read_buffer, ret);
}

bool					ClientSocket::check_body()
{
	std::string content_length = request.get_header_value("Content-Length");
	if (content_length == "" || int(buffer.size()) >= std::stoi(content_length))
	{
		request.set_request_body(buffer);
		return true;
	}
	return false;
}

bool					ClientSocket::check_headers()
{
	if (headers_complete)
		return true;
	
	while (true)
	{
		size_t found_line = buffer.find("\n");
		if (found_line == std::string::npos)
			break;
		
		std::string line = buffer.substr(0, found_line);
		buffer.erase(0, found_line + 1);

		bool is_first_line = request.get_request_line().http_version == "";

		if (is_first_line)
			request.parse_requestline(line);
		else if (request.parse_single_header_field(line))
		{
			request.validate_request();
			headers_complete = true;
			return true;
		}
	}
	return false;
}
