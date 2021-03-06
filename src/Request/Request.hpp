#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <iostream>
# include <vector>
# include <map>
# include <cstring>
# include <sys/socket.h>
# include <netinet/in.h>

class Request
{
	public:
		struct request_line
		{
			std::string method;
			std::string path;
			std::string query;
			std::string http_version;
		};

	private:
		struct request_line					req_line;
		std::map<std::string, std::string>	header_fields;
		std::string							body;
		std::string							plain_request;
		std::vector<std::string>			content_to_lines(std::string req);
		int									parse_header_fields(const std::vector<std::string> &lines);
		struct sockaddr_in					remote_address;
		Request();

	public:
		Request(struct sockaddr_in address);
		~Request();
		Request(const Request &src);
		Request										&operator=(const Request &rhs);
		void										set_request_body(const std::string &buffer);
		const struct request_line					&get_request_line() const;
		const std::map<std::string, std::string>	&get_header_fields() const;
		const std::string							&get_plain_request() const;
		friend std::ostream							&operator<<(std::ostream &Stream, const Request &request);
		const std::string							&get_header_value(const std::string& key) const;
		const std::string							&get_body() const;
		void										parse_requestline(std::string line);
		bool										parse_single_header_field(const std::string& line);
		int											validate_request();
		const struct sockaddr_in					&get_address() const;

};


#endif
