#include <mutex>

namespace cc::thread {

template <typename T>
class Mutex final
{
public:
    struct Data
    {
        T data;
        mutable std::mutex m;
    };

    struct LockRef
    {
        LockRef (Data & data) : data_ (data) {  data_.m.lock (); }
        LockRef (const LockRef &) = delete;
        LockRef (LockRef &&) = delete;
        ~ LockRef () { data_.m.unlock (); }

        T* operator -> () { return std::addressof (data_.data); }
        T& operator * () { return data_.data; }
    private:
        Data & data_;
    };
public:
    template<typename ... Args>
    Mutex (Args && ... args) noexcept (std::is_nothrow_constructible_v<T, Args ...>)
        : data_ {T (std::forward<Args> (args)...), std::mutex ()} { }

    auto lock () const { return LockRef (data_); }
    auto & unsafe_reference () { return data_.data; }
private:
    mutable Data data_;
};
}
