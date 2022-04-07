#include "SelectErrorException.hpp"

SelectErrorException::SelectErrorException() {}

SelectErrorException::~SelectErrorException() throw() {}

const char* SelectErrorException::what() const throw()
{
    return "Error occured at select: bad file descriptor\n";
}

std::ostream& operator<<(std::ostream& Stream, const SelectErrorException& SelectErrorException)
{
    Stream << SelectErrorException.what();
    return Stream;
}
