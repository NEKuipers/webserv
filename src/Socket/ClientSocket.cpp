#include "ClientSocket.hpp"
#include "Request.hpp"
#define BUFFER_SIZE 3000
#include <unistd.h>
#include <map>
#include "ReadFromSocketException.hpp"

// Constructor
ClientSocket::ClientSocket(struct sockaddr_in address, int sock) : SimpleSocket(address, sock), headers_complete(false), response(NULL)
{

}

ClientSocket::~ClientSocket()
{
	delete response;
}

bool					ClientSocket::read_body()
{
	std::string content_length = request.get_header_value("Content-Length");
	if (content_length == "")
		return true;
	else
	{
		char	read_buffer[BUFFER_SIZE];
		int ret = read(get_sock(), read_buffer, BUFFER_SIZE);
		if (ret < 0)
			throw ReadFromSocketException();
		buffer += std::string(read_buffer, ret);
		if (int(buffer.size()) >= std::stoi(content_length))
			request.set_request_body(buffer);
	}
	return true;
}

bool					ClientSocket::read_headers()
{
	while (true)
	{
		char	read_buffer[BUFFER_SIZE];
		int ret = read(get_sock(), read_buffer, BUFFER_SIZE);
		if (ret < 0)
		{
			throw ReadFromSocketException();
			break;
		}
		if (ret == 0)
			return false;
		buffer += std::string(read_buffer, ret);
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
	}
}

	// Step 1: Read next line
	// Step 2: If first line, call parse_requestline on request, otherwise, call parse_single_header_field, if it returns true, call validate_request() & return HEADER_COMPLETE
	// Step 3: If header is complete, wait till buffer has [content-length] of bytes, once it is, call set_request_body and return BODY_COMPLETE

Request		ClientSocket::get_request()
{
	return request;
}
