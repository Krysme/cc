#include <boost/thread/thread.hpp>

namespace cc::thread
{

class Thread final
{
    template<typename ... Args>
    friend Thread scoped (Args && ...);
public:
    template<typename ... Args>
    static Thread scoped (Args && ... args)
    {
        return Thread (std::forward<Args> (args)...);
    }
    template<typename ... Args>
    static void detached (Args && ... args)
    {
        boost::thread (std::forward<Args> (args)...).detach();
    }

    Thread (Thread && h) noexcept (std::is_nothrow_move_constructible_v<boost::thread>)
        :t_ (std::move (h.t_)) {  }
    Thread (const Thread &) = delete;

    void join ()
    {
        t_.join();
    }

    ~Thread ()
    {
        if (t_.joinable())  t_.join();
    }
private:
    template<typename ... Args>
    Thread (Args && ... args)  : t_ (std::forward<Args> (args)...) { }

private:
    boost::thread t_;
};




}
