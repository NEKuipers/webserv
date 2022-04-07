#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP
# include "ServerSocket.hpp"
# include "ClientSocket.hpp"
# include "Config.hpp"
# include "Request.hpp"
# include <csignal>
# include <unistd.h>
# include <cstdlib>
# include <stdexcept>
# include <vector>
# include <ctime>
# include <sys/select.h>

class WebServer
{
	private:
		std::vector<ServerSocket *>	accept_sockets;
		// std::vector<int> read_sockets;
		std::vector<ClientSocket *> read_sockets;


		int							connectionAccepter(ServerSocket *conn_socket);
		bool						connectionHandler(ClientSocket *conn_socket);
	public:
		WebServer();
		WebServer(int domain, int service, int protocol, int port, u_long interface, int bklg);
		WebServer(const WebServer &src);
		WebServer						&operator=(const WebServer &rhs);
		WebServer(Config &config);
		~WebServer();
		int							launch();
		ServerSocket				*get_socket();
		void						add_socket(ServerSocket socket);
		std::vector<ServerSocket *>	get_sockets();
		fd_set						get_socket_fd_set();
		int							get_highest_fd();
};

#endif
