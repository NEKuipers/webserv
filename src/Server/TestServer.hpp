#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP
# include "SimpleServer.hpp"
# include <csignal>
# include <unistd.h>
# include <cstdlib>
# include <stdexcept>

class TestServer: public SimpleServer
{
	private:
		char	buffer[30000];
		int		new_socket;
		void	connectionAccepter();
		void	connectionHandler();
		void	connectionResponder();
	public:
		TestServer();
		void	launch();

};

#endif
