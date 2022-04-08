#include <string>
#include <sstream>

template <typename T>
class has_print {
	private:
		template <typename U, U> struct really_has;

		template <typename C> static std::true_type Test(really_has <void (C::*)(std::ostream&) const, &C::Print>*);
		template <typename C> static std::true_type Test(really_has <void (C::*)(std::ostream&), &C::Print>*);

		template <typename> static std::false_type Test(...);

	public:
		typedef decltype(Test<T>(0)) enable;
};

template <typename T>
std::string to_string(typename std::enable_if<std::is_same<typename has_print<T>::enable, std::false_type>::value, const T&>::type Value)
{
	std::ostringstream Stream;
	Stream << Value;
	return Stream.str();
}

template <typename T>
std::string to_string(typename std::enable_if<has_print<T>::enable::value, const T&>::type Value)
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