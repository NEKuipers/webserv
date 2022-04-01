#include "ConnectionErrorException.hpp"

ConnectionErrorException::ConnectionErrorException() {}

ConnectionErrorException::~ConnectionErrorException() throw(){}

ConnectionErrorException::ConnectionErrorException(const ConnectionErrorException & copy) {
    *this = copy;
}

ConnectionErrorException      &ConnectionErrorException::operator=(const ConnectionErrorException & rhs) {
    static_cast <void> (rhs);
    return (*this);
}

const char       *ConnectionErrorException::what() const throw() {
    return ("Connection error.");
}

