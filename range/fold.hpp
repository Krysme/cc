#pragma once
#include <utility>
#include <cc/move.hpp>
#include <boost/range/numeric.hpp>

namespace cc::range
{

template<typename InputIter, typename Init, typename BinaryOperator>
inline Init
fold_impl_iter (InputIter first, InputIter one_past_last, Init init,
           BinaryOperator binary_op)
{
    for (; first != one_past_last; ++first)
        init = binary_op(std::move(init), *first);
    return init;
}

template<class SinglePassRange, class Init, class BinaryOperator = std::plus<void>>
inline Init fold_impl (const SinglePassRange& rng, Init init,
                       BinaryOperator op = {})
{
    return fold_impl_iter (boost::begin(rng), boost::end(rng), init, op);
}





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

    template<typename Init, typename BinaryOperator>
    constexpr FoldState<Init, BinaryOperator> operator () (Init && t, BinaryOperator && op) const
    {
        return FoldState<Init, BinaryOperator> {std::forward<Init> (t), std::forward<BinaryOperator> (op)};
    }

};

template <typename SinglePassRange, typename Init>
decltype (auto) operator | (SinglePassRange && rng, FoldState<Init> init)
{
    return fold_impl<SinglePassRange, Init> (rng, init.value);
}

template <typename SinglePassRange, typename Init, typename BinaryOperator>
decltype (auto) operator | (SinglePassRange && rng, FoldState<Init, BinaryOperator> holder)
{
    return fold_impl<SinglePassRange, Init, BinaryOperator> (rng, holder.value, holder.increment);
}


}
inline const detail::Fold fold;
}
