#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include <iostream>
# include <vector>
# include <cstring>
# include "ConfigResponse.hpp"
# include "Request.hpp"

class Response
{
    public:
		std::string response_string;

    public:
        Response(const std::string& response_string);
        ~Response();
        Response(const Response &src);

		static void			InitContentTypes();
		static Response*	generate_response(ConfigResponse *conf_response, Request &request);
		bool				is_response_complete();
		bool				cgi_write_available();
		bool				cgi_read_available();
		std::string			get_response_string() const;
		static std::string	get_reason_phrase(int status_code);
		static std::string	create_headers(ConfigResponse *conf_response, Request &request, int status_code);
};

#endif
