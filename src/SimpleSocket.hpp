#ifndef SIMPLESOCKET_HPP
# define SIMPLESOCKET_HPP
#include <sys/socket.h>
#include <netinet/in.h>

//created by following https://www.youtube.com/watch?v=YwHErWJIh6Y

class SimpleSocket
{
    private:
        struct sockaddr_in  address; //This struct is required to call bind()
        int                 connection;
        int                 sock;
    public:
        SimpleSocket(int domain, int service, int protocol, int port, u_long interface);
        ~SimpleSocket();
        virtual int         connect_to_network() = 0;

};

#endif
