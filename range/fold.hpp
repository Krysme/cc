#pragma once
#include <utility>
#include <boost/range/numeric.hpp>

namespace cc::range
{


namespace detail
{

template<typename ... Args>
struct FoldState;

template <typename Init>
struct FoldState<Init>
{
    static constexpr auto ___size___ = 1;
    Init value;
};

template <typename Init, typename Op>
struct FoldState<Init, Op>
{
    static constexpr auto ___size___ = 2;
    Init value;
    Op increment;
};

struct Fold
{
    template <typename Init>
    constexpr FoldState<Init> operator () (Init && t) const
    {
        return FoldState<Init> {std::forward<Init> (t)};
    }

    template<typename Init, typename Op>
    constexpr FoldState<Init, Op> operator () (Init && t, Op && op) const
    {
        return FoldState<Init, Op> {std::forward<Init> (t), std::forward<Op> (op)};
    }

};

template <typename SinglePassRange, typename Init>
decltype (auto) operator | (SinglePassRange && rng, FoldState<Init> init)
{
    return boost::accumulate (rng, init.value);
}

template <typename SinglePassRange, typename Init, typename Op>
decltype (auto) operator | (SinglePassRange && rng, FoldState<Init, Op> holder)
{
    return boost::accumulate (rng, holder.value, holder.increment);
}


}
inline const detail::Fold fold;
}
