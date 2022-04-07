#include "ClientSocket.hpp"
#include "Request.hpp"
#define BUFFER_SIZE 30000
#include <unistd.h>

// Constructor
ClientSocket::ClientSocket(struct sockaddr_in address, int sock) : SimpleSocket(address, sock)
{
	headers_complete = false;
}

enum read_status		ClientSocket::read_to_request() {

	// Read everything

	if (headers_complete)
	{
		// TODO NICK: Check if buffer.size() >= content-length, and if so, apply body & return
		return BODY_COMPLETE;
	}
	else
	{
		while (true)
		{
			char	read_buffer[BUFFER_SIZE];
			int ret = read(get_sock(), read_buffer, BUFFER_SIZE);
			if (ret < 0)
			{
				std::cout << "Not supposed to happen!" << std::endl;
				break;
			}
			// if (ret < 0)
			// 	throw ReadFromSocketException(); //TODO NICK make this
			if (ret == 0)
				return NOT_COMPLETE;
			
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
					return HEADER_COMPLETE;
				}
			}
		}
	}

	// Step 1: Read next line
	// Step 2: If first line, call parse_requestline on request, otherwise, call parse_single_header_field, if it returns true, call validate_request() & return HEADER_COMPLETE
	// Step 3: If header is complete, wait till buffer has [content-length] of bytes, once it is, call set_request_body and return BODY_COMPLETE
	
	return (NOT_COMPLETE); //TODO only return true if request is complete
}

Request		ClientSocket::get_request()
{
	return request;
}
