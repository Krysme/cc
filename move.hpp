#pragma once
#include <type_traits>
#include <ciso646>

namespace cc
{

template<typename T> decltype (auto) move (T& param)
{
    static_assert (not std::is_const<T>::value, "cannot apply move to a const value");

    using return_type = typename std::remove_reference<T>::type;
    return static_cast<return_type&&> (param);
}

template<typename T> decltype (auto) move (T&& param)
{
    static_assert (std::is_rvalue_reference<T>::value, "cannot apply move to an r-value");

    using return_type = typename std::remove_reference<T>::type;
    return static_cast<return_type&&> (param);
}
}
