#include "PipeException.hpp"

PipeException::PipeException() throw() {}
PipeException::PipeException(const PipeException& From) throw()
{
	this->operator=(From);
}

PipeException::~PipeException() throw()
{
}

PipeException& PipeException::operator = (const PipeException& From)
{
	(void)From;
	// return the existing object so we can chain this operator
	return *this;
}

const char* PipeException::what() const throw()
{
	return "PipeException";
}
std::ostream& operator<<(std::ostream& Stream, const PipeException& PipeException)
{
	Stream << PipeException.what();
	return Stream;
}
