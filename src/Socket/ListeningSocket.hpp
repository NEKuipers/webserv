#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP
#include "BindingSocket.hpp"

class ListeningSocket : public BindingSocket
{
	private:
		int backlog; //The maximum amount of queued connections
		int listening;
	public:
		ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int bklg);
		void 		start_listening();


};

#endif
