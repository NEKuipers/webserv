#include "Request.hpp"
#include "ParseRequestException.hpp"

//TODO: Fix exception throwing

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
}

int    Request::parse_header_fields(std::vector<std::string> lines)
{
    size_t i;
    size_t pos;

    for (i = 1; lines.size(); i++)
    {
        struct header_field new_field;

		if (lines[i] == "\r" || lines[i].length() == 0)
			return (i);
		if ((pos = lines[i].find(':')) == std::string::npos) 
			throw ParseRequestException("Incorrect formatting found in header.");
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
    }
    return (i);
}

static int validate_http_version(std::string http_version)
{
    std::string number = "0123456789";

	if (http_version.length() != 8)
		return (1);
	if (http_version[0] == 'H'
			&& http_version[1] == 'T' && http_version[2] == 'T'
			&& http_version[3] == 'P' && http_version[4] == '/'
			&& number.find(http_version[5]) != std::string::npos
			&& http_version[6] == '.'
			&& number.find(http_version[7]) != std::string::npos)
		return (0);
	return (1);
}

int     Request::validate_request()
{
    std::string methods[]={"GET", "POST", "DELETE"};
    if (std::find(std::begin(methods), std::end(methods), req_line.method) == std::end(methods))
        return (1);
    if (req_line.target[0] != '/')
        return (1);
    if (validate_http_version(req_line.http_version))
        return (1);
    return (0);
}

Request::Request(const std::string &request_content)
{
    plain_request = request_content;
    std::vector<std::string> lines = content_to_lines(plain_request);
    if (lines.size() < 1)
        throw ParseRequestException("Empty request.");
    parse_requestline(lines);
    size_t i = parse_header_fields(lines);
    while (i < lines.size() && (lines[i] == "\r" || lines[i].length() == 0))
        i++;
    std::vector<std::string>::iterator start = lines.begin() + i;
    if (start < lines.end())
        body = std::vector<std::string>(start, lines.end());
    if (validate_request())
        throw ParseRequestException("Invalid header content.");
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

std::string                         Request::get_header_value(std::string key)
{
    for (size_t i = 0; i < header_fields.size(); i++)
    {
        if (strcmp(header_fields[i].header_key.c_str(), key.c_str()) == 0)
            return (header_fields[i].header_value);
    }
    return NULL;
}


std::ostream                                &operator<<(std::ostream &Stream, const Request &request)
{
    Stream << "Request method is " << request.req_line.method << std::endl;
    Stream << "Requested file is " << request.req_line.target << std::endl;
    Stream << "HTTP version is " << request.req_line.http_version << std::endl;
    Stream << "Other header fields: " <<std::endl;
    for (size_t i = 0; i < request.header_fields.size(); i++)
    {
        Stream << "["<<request.header_fields[i].header_key << "] : [" << request.header_fields[i].header_value << "]" << std::endl;
    }
    if (request.body.size() > 1) {
        Stream << "Body: " <<std::endl;
        for (size_t i = 0; i < request.body.size(); i++) {
            Stream << request.body[i] << std::endl;
        }
    }
    return Stream;
}
