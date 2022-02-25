
#ifndef CONNECTION_ERROR_HPP
# define CONNECTION_ERROR_HPP
# include <stdexcept>

class ConnectionError : public std::exception
{
	public:
		ConnectionError();
		ConnectionError(const ConnectionError & copy);
		virtual ~ConnectionError() throw();
		ConnectionError  &operator=(const ConnectionError &rhs);
		virtual const char    *what() const throw();
};

#endif
