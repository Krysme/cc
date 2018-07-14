#pragma once
#include <gsl/span>


namespace cc
{
template <typename T, std::ptrdiff_t Extent = gsl::dynamic_extent>
using span = gsl::span<T, Extent>;
}
