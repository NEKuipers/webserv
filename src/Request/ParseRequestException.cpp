#include "ParseRequestException.hpp"

ParseRequestException::ParseRequestException(const std::string& Message) : Message(Message) {}

ParseRequestException::~ParseRequestException() throw() {}

const char* ParseRequestException::what () const throw () {
	return Message.c_str();
}
