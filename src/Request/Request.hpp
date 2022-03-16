//This class needs to function as an object to parse the contents of incoming http requests into
#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <iostream>
# include <vector>

class Request
{
    public:
        struct request_line
        {
            std::string method;
            std::string target;
            std::string http_version;
        };

        struct header_field
        {
            std::string header_key;
            std::string header_value;
        };

    private:
        struct request_line                 req_line;
        std::vector<struct header_field>    header_fields;
        std::string                         plain_request;
        std::vector<std::string>            content_to_lines(std::string req);
        void                                parse_requestline(std::vector<std::string> lines);
        int                                parse_header_fields(std::vector<std::string> lines);

    public:
        Request(const std::string &request_content);
        ~Request();
        Request(const Request &src);
        Request                             &operator=(const Request &rhs);
        struct request_line                 get_request_line();
        std::vector<struct header_field>    get_header_fields();
        std::string                         get_plain_request();

};

// std::ostream &operator<<(std::ostream &out, const Request &req);

#endif
