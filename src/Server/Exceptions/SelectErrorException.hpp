#ifndef SelectErrorException_HPP
#define SelectErrorException_HPP

#include <ostream>
#include <exception>

class SelectErrorException : public std::exception {
    public: 
        SelectErrorException();
        ~SelectErrorException() throw();

        const char* what() const throw();
        friend std::ostream& operator<<(std::ostream& Stream, const SelectErrorException& SelectErrorException);
};

#endif
