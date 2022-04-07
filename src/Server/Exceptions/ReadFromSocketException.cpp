#include "ReadFromSocketException.hpp"

ReadFromSocketException::ReadFromSocketException() {}

ReadFromSocketException::~ReadFromSocketException() throw()
{}

const char* ReadFromSocketException::what() const throw()
{
    return "Error: could not read from socket.";
}
std::ostream& operator<<(std::ostream& Stream, const ReadFromSocketException& ReadFromSocketException)
{
    Stream << ReadFromSocketException.what();
    return Stream;
}
