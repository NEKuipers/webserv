#include "MethodException.hpp"

MethodException::MethodException(const std::string& Type, const std::string& Method) : Type(Type), Method(Method) { }

MethodException::~MethodException() throw()
{

}

const char* MethodException::what () const throw () {
	return "Method was required to be accepted, but wasn't!";
}

std::ostream& operator<<(std::ostream& Stream, const MethodException& Exception)
{
	Stream << "Tried to put a '" << Exception.Type << "' In a place where method " << Exception.Method << " was not accepted!";
	return Stream;
}
