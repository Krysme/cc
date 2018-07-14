#include <utility>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <vector>

namespace cc::range
{

namespace detail
{

template <template <typename...> typename Container>
struct Collect
{
};

template <typename SinglePassRange, template <typename...> typename Container>
auto operator | (SinglePassRange && rng, Collect<Container>)
{
    using value_type = std::remove_const_t<std::remove_reference_t<std::remove_cv_t<decltype (*(std::begin (rng)))>>>;
    Container<value_type> v;

    for (auto && it : rng)
    {
        v.push_back (std::move (it));
    }

    return v;
}
}


template <template <typename...> class Container>
inline const detail::Collect<Container> collect;
}
