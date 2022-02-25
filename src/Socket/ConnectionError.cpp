#include "ConnectionError.hpp"

ConnectionError::ConnectionError() {}

ConnectionError::~ConnectionError() throw(){}

ConnectionError::ConnectionError(const ConnectionError & copy) {
    *this = copy;
}

ConnectionError      &ConnectionError::operator=(const ConnectionError & rhs) {
    static_cast <void> (rhs);
    return (*this);
}

const char       *ConnectionError::what() const throw() {
    return ("Connection error.");
}
