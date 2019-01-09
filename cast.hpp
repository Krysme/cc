#pragma once
#include <type_traits>
#include <gsl/span>



namespace cc
{

using std::underlying_type;
using std::is_pod;
using gsl::span;

template <typename Enum>
inline auto enum_cast (Enum e) -> typename underlying_type<Enum>::type
{
	return static_cast<typename underlying_type<Enum>::type>(e);
}

template <typename Pod>
inline span<char> byte_cast (Pod & pod)
{
	static_assert (is_pod<Pod>::value, "byte_cast cannot be applied to non-pod values");
	return span<char> (reinterpret_cast<char*>(&pod), sizeof (pod));
}

template <typename Pod>
inline span<const char> byte_cast (const Pod & pod)
{
	static_assert (is_pod<Pod>::value, "byte_cast cannot be applied to non-pod values");
	return span<const char> (reinterpret_cast<const char*>(&pod), sizeof (pod));
}


}
