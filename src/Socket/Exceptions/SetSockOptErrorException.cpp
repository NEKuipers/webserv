#include "SetSockOptErrorException.hpp"

SetSockOptErrorException::SetSockOptErrorException() {}


SetSockOptErrorException::~SetSockOptErrorException() throw() {}


const char* SetSockOptErrorException::what() const throw()
{
    return "Error: failed to set socket options properly\n";
}
std::ostream& operator<<(std::ostream& Stream, const SetSockOptErrorException& SetSockOptErrorException)
{
    Stream << SetSockOptErrorException.what();
    return Stream;
}
