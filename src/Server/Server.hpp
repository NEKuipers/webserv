#ifndef SERVER_HPP
# define SERVER_HPP
# include "ServerSocket.hpp"
# include <csignal>
# include <unistd.h>
# include <cstdlib>
# include <stdexcept>

class Server
{
	private:
		char				buffer[30000];
		ServerSocket 		*socket;
		int					new_socket;
		void				connectionAccepter();
		void				connectionHandler();
		void				connectionResponder();
	public:
		Server();
		Server(int domain, int service, int protocol, int port, u_long interface, int bklg);
		~Server();
		void				launch();
		ServerSocket		*get_socket();
};

#endif
