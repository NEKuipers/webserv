#ifndef ReadFromSocketException_HPP
#define ReadFromSocketException_HPP

#include <ostream>
#include <exception>

class ReadFromSocketException : public std::exception {
	public: 
		ReadFromSocketException();

		~ReadFromSocketException() throw();

		const char* what() const throw();
		friend std::ostream& operator<<(std::ostream& Stream, const ReadFromSocketException& ReadFromSocketException);
};

#endif
