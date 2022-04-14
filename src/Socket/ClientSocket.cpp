#include "ClientSocket.hpp"
#include "FileResponse.hpp"
#include "ErrorResponse.hpp"
#include "ToString.hpp"
#include "Request.hpp"
#include "Response.hpp"
#define BUFFER_SIZE 3000
#include <unistd.h>
#include <map>
#include "ReadFromSocketException.hpp"
#include "SendResponseException.hpp"

// Constructor
ClientSocket::ClientSocket(struct sockaddr_in address, int sock) : SimpleSocket(address, sock), headers_complete(false), written_size(0), conf_response(NULL)
{

}

ClientSocket::~ClientSocket()
{
	delete conf_response;
}

void 	ClientSocket::createResponse()
{
	Response http_response = Response(conf_response, request);
	to_write = http_response.get_response_string();
}

Request					ClientSocket::get_request()
{
	return request;
}

bool					ClientSocket::send()
{
	std::cerr << "========START OF RESPONSE==========\n";
	std::cerr << to_write.c_str() << std::endl;
	std::cerr << "==========END OF RESPONSE==========\n";
	int send_rv = ::send(get_sock(), to_write.c_str()+written_size, to_write.size()-written_size, 0);
	if (send_rv == -1)
		throw SendResponseException();
	written_size += send_rv;
	return written_size >= ssize_t(to_write.size());
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
