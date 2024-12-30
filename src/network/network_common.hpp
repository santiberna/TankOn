#include <boost/asio.hpp>
namespace asio = boost::asio;

template <typename T, typename Ret, typename... Args>
std::function<Ret(Args...)> BindMember(T* obj, Ret (T::*mem_fn)(Args...))
{
    return [obj, mem_fn](Args&&... args)
    { return (obj->*mem_fn)(std::forward<Args>(args)...); };
}