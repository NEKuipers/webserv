#include "ClientSocket.hpp"
#include "FileResponse.hpp"
#include "ErrorResponse.hpp"
#include "CgiResponse.hpp"
#include "ToString.hpp"
#include "Request.hpp"
#define BUFFER_SIZE 3000
#include <unistd.h>
#include <map>
#include "ReadFromSocketException.hpp"
#include "SendResponseException.hpp"

#include "CGIRunner.hpp"

// Constructor
ClientSocket::ClientSocket(struct sockaddr_in address, int sock) : SimpleSocket(address, sock), headers_complete(false), written_size(0), response(NULL)
{

}

ClientSocket::~ClientSocket()
{
	delete response;
}

void 	ClientSocket::createResponse()
{
	std::string Status;
	// TODO: Response headers
	std::string Headers = "";
	std::string ContentType = "";
	std::string Body;

	if (!response || dynamic_cast<ErrorResponse*>(response))
	{
		// I guess this is a default error page or something i dunno
		Status = "404 Not found";
		ContentType = "text/html";
		Body = "<!DOCTYPE html><html><p style=\"text-align:center;font-size:200%;\"><a href=\"/\">Webserv</a><br><br><b>Default error page<br>You seem to have made a invalid request!</b><br><p style=\"line-height: 5000em;text-align:right\"><b>h</b></div></p></html>";
	}
	else if (FileResponse* FileResponsePtr = dynamic_cast<FileResponse*>(response))
	{
		Status = "200 OK";
		ContentType = FileResponsePtr->GetContentType();
		Body = to_string(FileResponsePtr->GetStream().rdbuf());
	}
	else if (CgiResponse* CgiResponsePtr = dynamic_cast<CgiResponse*>(response))
	{
		std::map<std::string, std::string> map;
		CGIRunner runner(CgiResponsePtr->GetCgiFile(), map);

		//int stat;
		//pid_t pid = waitpid(Runner.CGIPid, &stat, 0);
		//assert(pid == Runner.CGIPid);

		// TODO: Non-Blocking read loop via select
		Status = "200 OK";
		to_write = "HTTP/1.1 " + Status + "\r\n";
		while (true)
		{
			char	read_buffer[BUFFER_SIZE];
			int read = ::read(runner.OutputFD, read_buffer, BUFFER_SIZE);
			std::cout << "Read: " << read << " bytes! " << std::endl;
			if (read <= 0)
				break;
			to_write += std::string(read_buffer, read);
		}
		std::cout << "to_write = " << to_write << std::endl;
		return;
	}
	else
	{
		Status = "200 OK";
		ContentType = "text/html";

		Body = "<!DOCTYPE html><html><p style=\"text-align:center;font-size:200%;\"><a href=\"/\">Webserv</a><br><br><b>Unknown response type:<br>";
		Body.append(to_string(*response));
		Body.append("</b><br><p style=\"line-height: 5000em;text-align:right\"><b>h</b></div></p></html>");
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
	int send_rv = ::send(get_sock(), to_write.c_str()+written_size, to_write.size()-written_size, 0);
	if (send_rv == -1)
		throw SendResponseException();
	written_size += send_rv;
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
