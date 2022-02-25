#ifndef SIMPLESOCKET_HPP
# define SIMPLESOCKET_HPP
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

//created by following https://www.youtube.com/watch?v=YwHErWJIh6Y

class SimpleSocket
{
    private:
        // This struct is required to call bind() or connect()
        struct sockaddr_in  address;
        int                 sock;

    public:
        SimpleSocket(int domain, int service, int protocol, int port, u_long interface);
        ~SimpleSocket(){};
        SimpleSocket(const SimpleSocket &src);
        SimpleSocket           &operator=(const SimpleSocket &rhs);

        // Connect_to_network() needs to be implemented as either bind or connect depending on server or client
        virtual int         connect_to_network(int sock, struct sockaddr_in address) = 0;

        // Function to test sockets in connections
        void                test_connection(int item_to_test);

        // Getter functions
        struct sockaddr_in  get_address();
        int                 get_sock();


};

#endif
