#include <string>
#include <sstream>

#include "SFINAEUtils.hpp"

template <typename T>
class has_print {
	private:
		template <typename U, U> struct really_has;

		template <typename C> static YesType& Test(really_has <void (C::*)(std::ostream&) const, &C::Print>*);
		template <typename C> static YesType& Test(really_has <void (C::*)(std::ostream&), &C::Print>*);

		template <typename> static NoType& Test(...);

	public:
		//typedef decltype(Test<T>(0)) enable;	// decltype is C++11 :(
		enum { value = sizeof(Test<T>(0)) == sizeof(YesType) };
};

template <typename T>
std::string to_string(typename enable_if<!has_print<T>::value, const T&>::type Value)
{
	std::ostringstream Stream;
	Stream << Value;
	return Stream.str();
}

template <typename T>
std::string to_string(typename enable_if<has_print<T>::value, const T&>::type Value)
{
	std::ostringstream Stream;
	Value.Print(Stream);
	return Stream.str();
}

// "Could not infer template argument", UGH, fine, have this function
template <typename T>
std::string to_string(const T& Value)
{
	return to_string<T>(Value);
}