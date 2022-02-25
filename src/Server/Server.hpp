#ifndef SERVER_HPP
# define SERVER_HPP
# include "ListeningSocket.hpp"
# include <csignal>
# include <unistd.h>
# include <cstdlib>
# include <stdexcept>

class Server
{
	private:
		char				buffer[30000];
		ListeningSocket 	*socket;
		int					new_socket;
		void				connectionAccepter();
		void				connectionHandler();
		void				connectionResponder();
	public:
		Server();
		Server(int domain, int service, int protocol, int port, u_long interface, int bklg);
		~Server();
		void				launch();
		ListeningSocket		*get_socket();

};

#endif
