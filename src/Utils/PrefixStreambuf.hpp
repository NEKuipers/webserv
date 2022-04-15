#ifndef PREFIXSTREAMBUF_HPP
#define PREFIXSTREAMBUF_HPP

// Slightly modified from https://stackoverflow.com/questions/9599807/how-to-add-indention-to-the-stream-operator

#include <streambuf>
#include <ostream>

class PrefixStreambuf : public std::streambuf
{
protected:
	virtual int overflow(int ch);
public:
	PrefixStreambuf(std::ostream& Dest, const std::string& Prefix);

	void End();

	virtual ~PrefixStreambuf();
	private:
		std::streambuf*     Dest;
		bool                IsAtStartOfLine;
		std::string         Prefix;
		std::ostream*       Owner;
};

#endif
