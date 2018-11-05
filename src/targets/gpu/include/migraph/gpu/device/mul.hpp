
#ifndef MIGRAPH_GUARD_RTGLIB_DEVICE_MUL_HPP
#define MIGRAPH_GUARD_RTGLIB_DEVICE_MUL_HPP

#include <migraph/argument.hpp>
#include <hip/hip_runtime_api.h>

namespace migraph {
namespace gpu {
namespace device {

void mul(hipStream_t stream, const argument& result, const argument& arg1, const argument& arg2);

void mul(hipStream_t stream,
         const argument& result,
         const argument& arg1,
         const argument& arg2,
         const argument& arg3);

} // namespace device
} // namespace gpu
} // namespace migraph

#endif
