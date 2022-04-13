#ifndef SendResponseException_HPP
#define SendResponseException_HPP

#include <ostream>
#include <exception>

class SendResponseException : public std::exception {
	public: 
		SendResponseException();

		~SendResponseException() throw();

		const char* what() const throw();
		friend std::ostream& operator<<(std::ostream& Stream, const SendResponseException& SendResponseException);
};

#endif
