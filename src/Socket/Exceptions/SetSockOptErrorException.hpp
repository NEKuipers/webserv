#ifndef SetSockOptErrorException_HPP
#define SetSockOptErrorException_HPP

#include <ostream>
#include <exception>

class SetSockOptErrorException : public std::exception {
    public: 
        SetSockOptErrorException();

        ~SetSockOptErrorException() throw();

        const char* what() const throw();
        friend std::ostream& operator<<(std::ostream& Stream, const SetSockOptErrorException& SetSockOptErrorException);
};

#endif
