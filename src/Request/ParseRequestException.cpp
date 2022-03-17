#include "ParseRequestException.hpp"

ParseRequestException::ParseRequestException(const std::string& Message) : Message(Message) {}

ParseRequestException::~ParseRequestException() _NOEXCEPT {}

const char* ParseRequestException::what () const throw () {
	return Message.c_str();
}
