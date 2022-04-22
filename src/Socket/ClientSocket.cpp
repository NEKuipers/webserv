#include "ClientSocket.hpp"
#include "ConfigFileResponse.hpp"
#include "ConfigErrorResponse.hpp"
#include "ConfigCgiResponse.hpp"
#include "ToString.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <unistd.h>
#include <map>
#include "ReadFromSocketException.hpp"
#include "SendResponseException.hpp"
#include <string>

#include "CGIRunner.hpp"

// Constructor
ClientSocket::ClientSocket(struct sockaddr_in address, int sock, struct sockaddr_in sock_adr) : SimpleSocket(address, sock), headers_complete(false), http_response(), conf_response(NULL)
{
	socket_address = sock_adr;
}

ClientSocket::~ClientSocket()
{
	delete conf_response;
	delete http_response;
}

void 	ClientSocket::createResponse()
{
	http_response = Response::generate_response(conf_response, request);
}

Request&				ClientSocket::get_request()
{
	return request;
}

void					ClientSocket::read(const std::string& read)
{
	buffer += read;
}

bool					ClientSocket::check_body()
{
	if (request.get_header_value("Transfer-Encoding").find("chunked") != std::string::npos)
	{
		while (true)
		{
			// Find \r\n to length
			size_t found = buffer.find("\r\n");
			if (found == std::string::npos) return false;

			size_t size = std::strtoul(buffer.c_str(), NULL, 16);
			if (buffer.length() < found + 2 + size + 2) return false;
			
			std::string Append = buffer.substr(found + 2, size);
			buffer.erase(0, found + 2 + size + 2);
			request.set_request_body(request.get_body() + Append);

			if (size == 0)
				return true;
		}
	}

	std::string content_length = request.get_header_value("Content-Length");
	if (content_length == "" || buffer.size() >= std::strtoul(content_length.c_str(), NULL, 10))	// NOTE: std::stoi() is C++11
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
			headers_complete = true;
			request.validate_request();
			return true;
		}
	}
	return false;
}

Response				*ClientSocket::get_http_response() const
{
	return http_response;
}
