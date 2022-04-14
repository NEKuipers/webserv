#ifndef WRITEEXCEPTION_HPP
#define WRITEEXCEPTION_HPP

#include <ostream>
#include <exception>

class WriteException : public std::exception {
	public: 
		WriteException() throw();
		WriteException(const WriteException& From) throw();

		~WriteException() throw();

		WriteException& operator = (const WriteException& From);

		const char* what() const throw();
		friend std::ostream& operator<<(std::ostream& Stream, const WriteException& WriteException);
};

#endif
