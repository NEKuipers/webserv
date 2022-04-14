#ifndef READEXCEPTION_HPP
#define READEXCEPTION_HPP

#include <ostream>
#include <exception>

class ReadException : public std::exception {
	public: 
		ReadException() throw();
		ReadException(const ReadException& From) throw();

		~ReadException() throw();

		ReadException& operator = (const ReadException& From);

		const char* what() const throw();
		friend std::ostream& operator<<(std::ostream& Stream, const ReadException& ReadException);
};

#endif
