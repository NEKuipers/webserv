#include "PrefixStreambuf.hpp"

PrefixStreambuf::PrefixStreambuf(std::ostream& Dest, const std::string& Prefix)
	: Dest(Dest.rdbuf()), IsAtStartOfLine(true), Prefix(Prefix), Owner(&Dest)
{
	Owner->rdbuf(this);
}

int PrefixStreambuf::overflow(int ch)
{
	if (IsAtStartOfLine && ch != '\n') {
		Dest->sputn(Prefix.data(), Prefix.size());
	}

	IsAtStartOfLine = ch == '\n';
	return Dest->sputc(ch);
}

void PrefixStreambuf::End()
{
	if (Owner != NULL) {
		Owner->rdbuf(Dest);
		Owner = NULL;
	}
}

PrefixStreambuf::~PrefixStreambuf()
{
	End();
}
