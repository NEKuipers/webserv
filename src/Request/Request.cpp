#include "Request.hpp"
#include "ParseRequestException.hpp"

std::vector<std::string> Request::content_to_lines(std::string req)
{
    std::vector<std::string> lines;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = req.find('\n', prev)) != std::string::npos)
    {
        lines.push_back(req.substr(prev, pos - prev));
        prev = pos + 1;
    }
    lines.push_back(req.substr(prev));
    return lines;
}

void    Request::parse_requestline(std::vector<std::string> lines)
{
	this->req_line.method = lines[0].substr(0, lines[0].find(' '));
	lines[0].erase(0, this->req_line.method.length() + 1);
	this->req_line.target = lines[0].substr(0, lines[0].find(' '));
	lines[0].erase(0, this->req_line.target.length() + 1);
	if (lines[0].find('\r') != std::string::npos)
		this->req_line.http_version = lines[0].substr(0, lines[0].find('\r'));
	else if (lines[0].find('\n') != std::string::npos)
		this->req_line.http_version = lines[0].substr(0, lines[0].find('\n'));
	else
		this->req_line.http_version = lines[0].substr(0, lines[0].find(' '));
    //NEEDS VALIDATION 
}

int    Request::parse_header_fields(std::vector<std::string> lines)
{
    size_t i;

    for (i = 1; lines.size(); i++)
    {
        struct header_field new_field;
        size_t pos;

		if (lines[i] == "\r" || lines[i].length() == 0)
			return (i);
		if ((pos = lines[i].find(':')) == std::string::npos) 
			throw ParseRequestException();
		new_field.header_key = lines[i].substr(0, pos);
		lines[i].erase(0, pos + 1);
		
		if (lines[i][0] == ' ')
			lines[i].erase(0, 1);

		if (lines[i].find('\r') != std::string::npos)
			new_field.header_value = lines[i].substr(0, lines[i].find('\r'));
		else if (lines[i].find('\n') != std::string::npos)
			new_field.header_value = lines[i].substr(0, lines[i].find('\n'));
		else
			new_field.header_value = lines[i].substr(0, lines[i].length());

        this->header_fields.push_back(new_field);
        //NEEDS VALIDATION
    }
    return (i);
}

Request::Request(const std::string &request_content)
{
    plain_request = request_content;
    std::vector<std::string> lines = content_to_lines(plain_request);
    if (lines.size() < 1)
        throw ParseRequestException();
    parse_requestline(lines);
    parse_header_fields(lines);
}

Request::Request(const Request &src)
{
    *this = src;
}

Request     &Request::operator=(const Request &rhs)
{
    this->req_line = rhs.req_line;
    this->header_fields = rhs.header_fields;
    this->plain_request = rhs.plain_request;
    return (*this);
}

Request::~Request(){}

struct Request::request_line                 Request::get_request_line()
{
    return this->req_line;
}

std::vector<struct Request::header_field>    Request::get_header_fields()
{
    return this->header_fields;
}

std::string                         Request::get_plain_request() 
{
    return this->plain_request;
}
