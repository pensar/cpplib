#ifndef CODE_UTIL_HPP
#define CODE_UTIL_HPP

#include "s.hpp"

namespace pensar_digital 
{
    namespace cpplib 
    {

#if defined(_WIN32) || defined(_WIN64)
#define _PLATFORM_FOLDER windows
#define _PLATFORM_SUFFIX windows
#define _INCLUDE_PATH(base_name) _STRINGIFY(windows/base_name##_windows.hpp)
#define INCLUDE(base_name) _INCLUDE_PATH(base_name)
#elif defined(__linux__)
#define _PLATFORM_FOLDER linux
#define _PLATFORM_SUFFIX linux
#define _INCLUDE_PATH(base_name) _STRINGIFY(linux/base_name##_linux.hpp)
#define INCLUDE(base_name) _INCLUDE_PATH(base_name)
#elif defined(__APPLE__) && defined(TARGET_OS_IPHONE)
#define _PLATFORM_FOLDER ios
#define _PLATFORM_SUFFIX ios
#define _INCLUDE_PATH(base_name) _STRINGIFY(ios/base_name##_ios.hpp)
#define INCLUDE(base_name) _INCLUDE_PATH(base_name)
#elif defined(__ANDROID__)
#define _PLATFORM_FOLDER android
#define _PLATFORM_SUFFIX android
#define _INCLUDE_PATH(base_name) _STRINGIFY(android/base_name##_android.hpp)
#define INCLUDE(base_name) _INCLUDE_PATH(base_name)
#else
#error "Unsupported platform"
#endif

        // Helper macro for stringification
#define _STRINGIFY(x) #x



        template <typename ResultType>
        struct Result {
            ResultType mresult;
            bool mok;
            S merror_message;

            Result(ResultType result, bool ok, S error_message)
                : mresult(result), mok(ok), merror_message(error_message) {
            }

            // Implicit conversion to bool
            operator bool() const { return mok; }

            // Implicit conversion to ResultType
            operator ResultType() const { return mresult; }

            // Implicit conversion to S
            operator S() const { return merror_message; }
        };

    }  // namespace cpplib
}  // namespace pensar_digital

#endif // CODE_UTIL_HPP