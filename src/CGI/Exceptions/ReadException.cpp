#include "ReadException.hpp"

ReadException::ReadException() throw() {}
ReadException::ReadException(const ReadException& From) throw()
{
	this->operator=(From);
}

ReadException::~ReadException() throw()
{
	// TODO: Implement destructor
}

ReadException& ReadException::operator = (const ReadException& From)
{
	(void)From;
	// return the existing object so we can chain this operator
	return *this;
}

const char* ReadException::what() const throw()
{
	return "ReadException";
}
std::ostream& operator<<(std::ostream& Stream, const ReadException& ReadException)
{
	Stream << ReadException.what(); // TODO: Better logging
	return Stream;
}
