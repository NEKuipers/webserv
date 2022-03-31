#ifndef PARSEREQUESTEXCEPTION_HPP
# define PARSEREQUESTEXCEPTION_HPP
# include <exception>
# include <string>
# include <ostream>

struct ParseRequestException : public std::exception
{
	const std::string Message;

	ParseRequestException(const std::string& Message);

	virtual ~ParseRequestException() _NOEXCEPT;

	const char* what () const throw ();
};

#endif
