#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include <iostream>
# include <vector>
# include <cstring>
# include "ConfigResponse.hpp"

class Response
{
    public:


		std::string response_string;

    public:
        Response(ConfigResponse *conf_response);
        ~Response();
        Response(const Response &src);

		void		InitContentTypes();
		std::string	get_response_string();
};

#endif
