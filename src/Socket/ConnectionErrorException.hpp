
#ifndef CONNECTIONERROREXCEPTION_HPP
# define CONNECTIONERROREXCEPTION_HPP
# include <stdexcept>

class ConnectionErrorException : public std::exception
{
	public:
		ConnectionErrorException();
		ConnectionErrorException(const ConnectionErrorException & copy);
		virtual ~ConnectionErrorException() throw();
		ConnectionErrorException  &operator=(const ConnectionErrorException &rhs);
		virtual const char    *what() const throw();
};

#endif
