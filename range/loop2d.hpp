#pragma once
#include <type_traits>
#include <utility>
namespace cc::range
{
namespace detail
{

using std::invoke_result_t;
using std::is_same_v;
using std::is_integral_v;
using std::pair;


using CountingType = int;

template <typename T>
struct IntegerRange
{
    static_assert (is_integral_v<T>);
    T begin;
    T end;
};

struct Loop2dState
{
    int x_begin, x_end;
    int y_begin, y_end;
};

struct Loop2d
{
    constexpr Loop2dState operator () (int x, int y) const
    {
        return Loop2dState {0, x, 0, y};
    }

};

struct Iter2dState
{
    CountingType x_begin, x_end;
    CountingType y_begin, y_end;
    struct IterEnd {};
    struct IterType
    {
        std::pair<CountingType, CountingType> operator*()
        {
            return data;
        }

        void operator++(int)
        {
            operator++();
        }

        void operator++()
        {
            data.first ++;
            if (data.first < x_end)
            {
                return;
            }
            else
            {
                data.second ++;
                data.first = x_begin;
            }
        }
        bool operator==(IterEnd)
        {
            return data.second >= y_end;
        }
        std::pair<CountingType, CountingType> data;
        CountingType x_begin, x_end;
        CountingType y_end;
    };

    using iterator = IterType;

    IterType begin ()
    {
        return IterType {{x_begin, y_begin}, x_begin, x_end, y_end};
    }

    IterEnd end ()
    {
        return IterEnd {};
    }
};

struct Iter2d
{
    constexpr Iter2dState operator () (int x, int y) const
    {
        return Iter2dState {0, x, 0, y};
    }
};

template <typename T>
void operator| (Loop2dState l, T && t)
{
    for (auto i = l.x_begin; i < l.x_end; i ++)
    {
        for (auto j = l.y_begin; j < l.y_end; j ++)
        {
            if constexpr (is_same_v<invoke_result_t<T, decltype (i), decltype (j)>, void>)
            {
                t (i, j);
            }
            else
            {
                if (not t (i, j))
                {
                    break;
                }
            }
        }
    }
}

}

inline const detail::Loop2d loop2d;
inline const detail::Iter2d iter2d;
}
