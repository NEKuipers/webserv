#ifndef SIMPLESOCKET_HPP
# define SIMPLESOCKET_HPP
#include <sys/socket.h>
#include <netinet/in.h>
#include "ConnectionErrorException.hpp"
#include <iostream>
#include <cstdlib>

class SimpleSocket
{
    private:
        // This struct is required to call bind() or connect()
        struct sockaddr_in  address;
        int                 sock;

    public:
        SimpleSocket(int domain, int service, int protocol, int port, u_long interface);
        SimpleSocket(struct sockaddr_in address, int sock);
        virtual ~SimpleSocket();
        SimpleSocket(const SimpleSocket &src);
        SimpleSocket           &operator=(const SimpleSocket &rhs);

        // Function to test sockets in connections
        void                test_connection(int item_to_test);

        // Getter functions
        struct sockaddr_in  get_address();
        int                 get_sock();

        static void         close_all();


};

#endif
