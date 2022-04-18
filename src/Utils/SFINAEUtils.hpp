#pragma once

// Ugh, why must i be in old C++98 land

// Here, have a C++98 enable_if
template<bool B, class T = void>
struct enable_if {};

template<class T>
struct enable_if<true, T> { typedef T type; };

// And a bad C++98 true/false type
typedef char YesType[1];
typedef char NoType[2];

// and a C++98 is_same
template<class T, class U>
struct is_same { static const bool value = false; };

template<class T>
struct is_same<T, T> { static const bool value = true; };