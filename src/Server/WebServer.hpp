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
# include <Selector.hpp>
class WebServer
{
	private:
		struct CgiReadData
		{
			WebServer* Server;
			ClientSocket* Socket;
			std::string Read;

			CgiReadData(WebServer* Server, ClientSocket* Socket);
		};
		
		Selector					selector;

		std::vector<ServerSocket *>	server_sockets;
		std::vector<ClientSocket *> client_sockets;

		Config*						configuration;

		bool						IsRequestComplete(ClientSocket *conn_socket);
		bool						connectionResponder(ClientSocket *conn_socket);


		static bool					onAccept(std::pair<WebServer*, ServerSocket*>* Arg, int ClientFD, const struct sockaddr& Address, socklen_t AddressLen);
		static bool					onRead(std::pair<WebServer*, ClientSocket*>* Arg, bool LastRead, const std::string& Read);
		static bool					onCgiRead(struct CgiReadData* Arg, bool LastRead, const std::string& Read);
		static bool					onWriteCloseAfterComplete(ClientSocket* Arg, bool LastWrite, int StartByte, int NumBytes);

		WebServer(const WebServer &src);
		WebServer						&operator=(const WebServer &rhs);
	public:
		WebServer();
		WebServer(int domain, int service, int protocol, int port, u_long interface, int bklg);
		WebServer(Config &config);
		~WebServer();
		int							launch();
		ServerSocket				*get_socket();
		void						add_socket(ServerSocket socket);
		std::vector<ServerSocket *>	get_sockets();
};

#endif
