#include "ConvertException.hpp"
#include <fstream>
#include <iostream>
#include <istream>

ConvertException::ConvertException(const std::string& From, const std::string& To, const std::string& Reason) : From(From), To(To), Reason(Reason) { }

ConvertException::~ConvertException() throw()
{

}

const char* ConvertException::what () const throw () {
	return Reason.c_str();
}

std::ostream& operator<<(std::ostream& Stream, const ConvertException& Exception)
{
	std::cerr << Stream.tellp() << std::endl;
	return Stream << "Could not convert '" + Exception.From + "' into '" + Exception.To + "': " + Exception.Reason;	// Why can't i replace the + with <<?
}
