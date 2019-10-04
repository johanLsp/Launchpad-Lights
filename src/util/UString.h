// Copyright 2019 Johan Lasperas
#ifndef SRC_UTIL_USTRING_H_
#define SRC_UTIL_USTRING_H_

#include <string>

// Define an unsigned char string type.
typedef std::basic_string<uint8_t> ustring;

// User-defined unsigned char literal.
inline constexpr unsigned char operator ""_u(unsigned long long arg) noexcept {
    return static_cast<unsigned char>( arg );
}

#endif  // SRC_UTIL_USTRING_H_
