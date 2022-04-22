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

static std::string split_untill(std::string& base, char split)
{
	size_t found = base.find(split);
	if (found == std::string::npos)
		found = base.length();
	
	std::string ret = base.substr(0, found);
	base.erase(0, found + 1);
	return ret;
}
static std::string split_untill_first(std::string& base, const std::string& split)
{
	size_t found = base.find_first_of(split);
	if (found == std::string::npos)
		found = base.length();
	
	std::string ret = base.substr(0, found);
	base.erase(0, found + 1);
	return ret;
}

void    Request::parse_requestline(std::string line)
{
	this->req_line.method = split_untill(line, ' ');
	std::string target = split_untill(line, ' ');
	this->req_line.http_version = split_untill_first(line, "\r\n ");
	
	this->req_line.path = split_untill(target, '?');
	this->req_line.query = target;
}

bool   Request::parse_single_header_field(const std::string &line)
{
	if (line == "\r" || line.length() == 0)
		return (true);
	size_t pos = line.find(':');
	if (pos == std::string::npos)
		throw ParseRequestException("Incorrect formatting found in header.");
	std::string header_key = line.substr(0, pos);
	pos++;
	if (line[pos] == ' ')
		pos++;
	std::string header_value;
	header_value = line.substr(pos, line.find_first_of("\r\n ", pos) - pos);
	this->header_fields.insert(std::make_pair(header_key, header_value));
	return (false);
}

int    Request::parse_header_fields(const std::vector<std::string> &lines)
{
	size_t i;

	for (i = 1; lines.size(); i++)
	{
		bool last_line = parse_single_header_field(lines[i]);
		if (last_line)
			return (i);
	}
	return (i);
}

static int validate_http_version(const std::string& http_version)
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
	std::string methods[] = { "GET", "POST", "DELETE" };
	//if (std::find(std::begin(methods), std::end(methods), req_line.method) == std::end(methods))	// std::begin() is C++11
	//	return (1);
	for (size_t i = 0; i < sizeof(methods) / sizeof(methods[0]); i++)
		if (methods[i] == req_line.method)
			return 1;
	if (req_line.path[0] != '/')
		return (1);
	if (validate_http_version(req_line.http_version))
		return (1);
	return (0);
}

void	Request::set_request_body(const std::string &buffer)
{
	body = buffer;
}

Request::Request(struct sockaddr_in address) : remote_address(address)
{
	
}

Request::Request(const Request &src)
{
	*this = src;
}

Request								&Request::operator=(const Request &rhs)
{
	this->req_line = rhs.req_line;
	this->header_fields = rhs.header_fields;
	this->plain_request = rhs.plain_request;
	return (*this);
}

Request::~Request(){}

const struct Request::request_line		&Request::get_request_line() const
{
	return req_line;
}

const std::map<std::string, std::string>    &Request::get_header_fields() const
{
	return this->header_fields;
}

const std::string                         &Request::get_plain_request()  const
{
	return this->plain_request;
}

const static std::string NoHeaderValue = "";	// Not great, but eh, const-correct
const std::string                         &Request::get_header_value(const std::string &key) const
{
	std::map<std::string, std::string>::const_iterator it = header_fields.find(key);
	if (it == header_fields.end())
		return NoHeaderValue;
	return it->second;
}


std::ostream                                &operator<<(std::ostream &Stream, const Request &request)
{
	Stream << "Request method is " << request.req_line.method << std::endl;
	Stream << "Requested file is " << request.req_line.path << std::endl;
	Stream << "HTTP version is " << request.req_line.http_version << std::endl;
	Stream << "Other header fields: " << std::endl;

	for (std::map<std::string, std::string>::const_iterator it = request.header_fields.begin(); it != request.header_fields.end(); it++)
		Stream << "[" << it->first << "] : [" << it->second << "]" << std::endl;

	if (request.body.size() > 1) {
		Stream << "Body: " << std::endl;
		Stream << request.body << std::endl;
	}
	return Stream;
}

const std::string							&Request::get_body() const
{
	return body;
}

const struct sockaddr_in					&Request::get_address() const
{
	return remote_address;
}
