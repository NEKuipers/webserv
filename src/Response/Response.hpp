#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include <iostream>
# include <vector>
# include <cstring>

class Response
{
    public:
        struct status_line
        {
            std::string http_version;// = "HTTP/1.1";
            std::string status_code;
            std::string reason_phrase;
        };

    public:
        Response();
        ~Response();
        Response(const Response &src);
        // Response        &operator=(const Response &rhs);
};

#endif
