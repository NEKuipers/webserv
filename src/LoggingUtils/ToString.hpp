#include <string>
#include <sstream>

// Apparently std::to_string is c++11, and not available on linux, so here, another way
template <typename T>
std::string to_string(const T& Value)
{
	std::ostringstream Stream;
	Stream << Value;
	return Stream.str() ;
}