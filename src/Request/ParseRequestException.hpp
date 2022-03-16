#ifndef PARSEREQUESTEXCEPTION_HPP
# define PARSEREQUESTEXCEPTION_HPP
# include <exception>
# include <string>
# include <ostream>

struct ParseRequestException : public std::exception
{
	ParseRequestException();
	virtual ~ParseRequestException() _NOEXCEPT;
	const char* what () const throw ();
};

#endif
