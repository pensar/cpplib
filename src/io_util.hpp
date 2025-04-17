// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)
#ifndef IO_UTIL_HPP
#define IO_UTIL_HPP

#include "code_util.hpp"
#include "defines.hpp"
#include "s.hpp"
#include "memory.hpp"
#include "constant.hpp"
#include "concept.hpp"
#include "icu_util.hpp"
#include "log.hpp"
#include "error.hpp"
        // Include platform-specific implementation
#include INCLUDE(io_util)

// or "linux/io_util_linux.hpp" 
// or "android/io_util_android.hpp" or other platforms

namespace pensar_digital {
    namespace cpplib {

        using LINE_HANDLER = void(*)(const int64_t line_count, const S& line);
        // Forward declaration
        Result<S> get_exe_full_path();


    }  // namespace cpplib
}  // namespace pensar_digital

#endif // IO_UTIL_HPP

