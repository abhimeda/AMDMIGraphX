
#include <rtg/miopen/hip.hpp>

#include <rtg/manage_ptr.hpp>
#include <miopen/miopen.h>

#include <vector>

namespace rtg {
namespace miopen {

using hip_ptr = RTG_MANAGE_PTR(void, hipFree);

hip_ptr allocate_gpu(std::size_t sz)
{
    void* result;
    // TODO: Check status
    hipMalloc(&result, sz);
    return hip_ptr{result};
}

template <class T>
hip_ptr write_to_gpu(const T& x)
{
    using type = typename T::value_type;
    auto size  = x.size() * sizeof(type);
    return write_to_gpu(x.data(), size);
}

template <class T>
std::vector<T> read_from_gpu(const void* x, std::size_t sz)
{
    std::vector<T> result(sz);
    // TODO: Check status
    hipMemcpy(result.data(), x, sz * sizeof(T), hipMemcpyDeviceToHost);
    return result;
}

hip_ptr write_to_gpu(const void* x, std::size_t sz)
{
    auto result = allocate_gpu(sz);
    // TODO: Check status
    hipMemcpy(result.get(), x, sz, hipMemcpyHostToDevice);
    return result;
}

rtg::argument allocate_gpu(rtg::shape s)
{
    auto p = share(allocate_gpu(s.bytes()));
    return {s, [p]() mutable { return reinterpret_cast<char*>(p.get()); }};
}

rtg::argument to_gpu(rtg::argument arg)
{
    auto p = share(write_to_gpu(arg.data(), arg.get_shape().bytes()));
    return {arg.get_shape(), [p]() mutable { return reinterpret_cast<char*>(p.get()); }};
}

rtg::argument from_gpu(rtg::argument arg)
{
    rtg::argument result;
    arg.visit([&](auto x) {
        using type = typename decltype(x)::value_type;
        auto v     = read_from_gpu<type>(arg.data(), x.get_shape().bytes() / sizeof(type));
        result     = {x.get_shape(), [v]() mutable { return reinterpret_cast<char*>(v.data()); }};
    });
    return result;
}

} // namespace miopen

} // namespace rtg