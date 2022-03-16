#include "ConvertException.hpp"

ConvertException::ConvertException(const std::string& Message) : Message(Message) { }

ConvertException::~ConvertException() _NOEXCEPT
{

}

const char* ConvertException::what () const throw () {
	return Message.c_str();
}
