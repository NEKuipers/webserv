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

    public:
        Response();
        ~Response();
        Response(const Response &src);

		static void			InitContentTypes();
		static Response*	generate_response(ConfigResponse *conf_response, Request &request);
		static std::string	get_reason_phrase(int status_code);
		static std::string	create_status_line(int status_code);
		static std::string	create_headers(ConfigResponse *conf_response, Request &request, int status_code);
		

		virtual bool		get_response_string(std::string &response_string) = 0;
};

#endif
