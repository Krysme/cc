#pragma once
#include <utility>

enum class __scope_guard_on_exit__ {};

template<typename CALLABLE>
struct __scope_guard__
{
    CALLABLE c_;
    __scope_guard__ (CALLABLE&& c) : c_ (std::forward<CALLABLE> (c)) {}
    ~__scope_guard__ ()
    {
        c_ ();
    }
};

template<typename fun>
__scope_guard__<fun> operator + (__scope_guard_on_exit__, fun&& fn)
{
    return __scope_guard__<fun> (std::forward<fun> (fn));
}

#define ___CONCAT_IMPL___(str1,str2) str1##str2
#define CONCAT(str1,str2) ___CONCAT_IMPL___(str1,str2)

#define __ANONYMOUS_VAR__(str) CONCAT(str,__LINE__)

/// 当退出作用域时调用 使用格式为 SCOPE_EXIT { fclose (fp);};
#define SCOPE_EXIT \
    auto __ANONYMOUS_VAR__(HELPER_SCOPE_EXIT_STATE) \
= __scope_guard_on_exit__ () + [&]()
