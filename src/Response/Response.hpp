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
        Response(ConfigResponse *conf_response, Request &request);
        ~Response();
        Response(const Response &src);

		void		InitContentTypes();
		std::string	get_response_string();
		std::string	get_reason_phrase(int status_code);
};

#endif
