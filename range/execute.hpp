#pragma once
#include <utility>

namespace cc::range
{
namespace detail
{
template<typename Op>
struct ExecuteState
{
    Op lambda;
};

struct Execute
{
    template <typename Op>
    constexpr ExecuteState<Op> operator () (Op && t) const
    {
        return ExecuteState<Op> {std::forward<Op> (t)};
    }

};

template <typename SinglePassRange, typename Op>
void operator | (SinglePassRange && rng, ExecuteState<Op> holder)
{
    for (auto && it : rng)
    {
        holder.lambda (std::forward<decltype (it)> (it));
    }
}

}

inline const detail::Execute execute;
}
