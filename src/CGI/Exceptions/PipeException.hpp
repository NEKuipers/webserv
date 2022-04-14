#ifndef PIPEEXCEPTION_HPP
#define PIPEEXCEPTION_HPP

#include <ostream>
#include <exception>

class PipeException : public std::exception {
	public: 
		PipeException() throw();
		PipeException(const PipeException& From) throw();

		~PipeException() throw();

		PipeException& operator = (const PipeException& From);

		const char* what() const throw();
		friend std::ostream& operator<<(std::ostream& Stream, const PipeException& PipeException);
};

#endif
