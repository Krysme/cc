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
    using namespace std;
    using value_type = remove_const_t<remove_reference_t<remove_cv_t<decltype (*(begin (rng)))>>>;
    Container<value_type> v;

    for (auto && it : rng)
    {
        v.push_back (std::move (it));
    }
    //boost::push_back (v, rng);

    return v;
}
}


template <template <typename...> class Container>
inline const detail::Collect<Container> collect;
}
