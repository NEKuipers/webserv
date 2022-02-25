#ifndef SIMPLESERVER_HPP
# define SIMPLESERVER_HPP
# include "ListeningSocket.hpp"

class SimpleServer
{
	private:
		ListeningSocket *socket;
		virtual void 	connectionAccepter() = 0;
		virtual void 	connectionHandler() = 0;
		virtual void	connectionResponder() = 0;
	public:
		SimpleServer(int domain, int service, int protocol, int port, u_long interface, int bklg);
		~SimpleServer();
		virtual void	launch() = 0;
		ListeningSocket	*get_socket();
};

#endif
