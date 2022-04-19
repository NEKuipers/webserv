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
        virtual ~Response();
        Response(const Response &src);

		static void			InitStatusCodes();
		static Response*	generate_response(ConfigResponse *conf_response, Request &request);
		static std::string	get_reason_phrase(int status_code);
		static std::string	create_status_line(int status_code);
		static std::string	create_headers(ConfigResponse *conf_response, Request &request, int status_code);
		static int			delete_method(const std::string& fullpath);
		static int			create_method(const std::string& fullpath, const std::string& contents);
};

#endif
