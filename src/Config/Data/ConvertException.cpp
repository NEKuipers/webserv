#include "ConvertException.hpp"

ConvertException::ConvertException(const std::string& Message) : Message(Message) { }

const char* ConvertException::what () const throw () {
	return Message.c_str();
}
