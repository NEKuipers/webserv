#ifndef SERVER_HPP
# define SERVER_HPP
# include "ServerSocket.hpp"
# include <csignal>
# include <unistd.h>
# include <cstdlib>
# include <stdexcept>
# include <vector>

class Server
{
	private:
		char						buffer[30000];
		ServerSocket 				*socket;
		std::vector<ServerSocket>	sockets;
		
		int							sock_fd;
		void						connectionAccepter();
		void						connectionHandler();
		void						connectionResponder();
		void						connectionCloser();
	public:
		Server();
		Server(int domain, int service, int protocol, int port, u_long interface, int bklg);
		Server(const Server &src);
		Server						&operator=(const Server &rhs);
		//Server(Configuration) Er moet een constructor komen die het config-object omzet in socket(s)
		~Server();
		void						launch();
		ServerSocket				*get_socket();
		void						add_socket(ServerSocket socket);
		std::vector<ServerSocket>	get_sockets();
};

#endif
