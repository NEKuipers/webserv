#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP
# include "SimpleServer.hpp"
# include <unistd.h>

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
