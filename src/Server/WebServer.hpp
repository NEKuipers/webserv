#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP
# include "ServerSocket.hpp"
# include "Request.hpp"
# include <csignal>
# include <unistd.h>
# include <cstdlib>
# include <stdexcept>
# include <vector>
# include <ctime>
# include <sys/select.h>
# define BUFFER_SIZE 30000

class WebServer
{
	private:
		std::vector<ServerSocket *>	sockets;
		
		void						connectionAccepter(ServerSocket *conn_socket);
		int							connectionHandler(ServerSocket *conn_socket);
		void						connectionResponder(ServerSocket *conn_socket);
		void						connectionCloser(ServerSocket *conn_socket);
	public:
		WebServer();
		WebServer(int domain, int service, int protocol, int port, u_long interface, int bklg);
		WebServer(const WebServer &src);
		WebServer						&operator=(const WebServer &rhs);
		//WebServer(Configuration) Er moet een constructor komen die het config-object omzet in socket(s)
		~WebServer();
		int							launch();
		ServerSocket				*get_socket();
		void						add_socket(ServerSocket socket);
		std::vector<ServerSocket *>	get_sockets();
		fd_set						get_socket_fd_set();
		int							get_highest_fd();
};

#endif
