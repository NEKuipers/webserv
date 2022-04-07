#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <iostream>
# include <vector>
# include <map>
# include <cstring>

class Request
{
    public:
        struct request_line
        {
            std::string method;
            std::string target;
            std::string http_version;
        };

    private:
        struct request_line                 req_line;
        std::map<std::string, std::string>  header_fields;
        std::vector<std::string>            body;
        std::string                         plain_request;
        std::vector<std::string>            content_to_lines(std::string req);
        void                                parse_requestline(std::vector<std::string> lines);
        int                                 parse_header_fields(std::vector<std::string> lines);
        int                                 validate_request();

    public:
        Request(const std::string &request_content);
        ~Request();
        Request(const Request &src);
        Request                             &operator=(const Request &rhs);
        struct request_line                 get_request_line();
        std::map<std::string, std::string>  get_header_fields();
        std::string                         get_plain_request();
        friend std::ostream                 &operator<<(std::ostream &Stream, const Request &request);
        std::string                         get_header_value(std::string key);

};


#endif
