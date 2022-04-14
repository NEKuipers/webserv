#include "WriteException.hpp"

WriteException::WriteException() throw() {}
WriteException::WriteException(const WriteException& From) throw()
{
	this->operator=(From);
}

WriteException::~WriteException() throw()
{
	// TODO: Implement destructor
}

WriteException& WriteException::operator = (const WriteException& From)
{
	// TODO: do the copy
	(void)From;

	// return the existing object so we can chain this operator
	return *this;
}

const char* WriteException::what() const throw()
{
	return "WriteException";
}
std::ostream& operator<<(std::ostream& Stream, const WriteException& WriteException)
{
	Stream << WriteException.what(); // TODO: Better logging
	return Stream;
}
