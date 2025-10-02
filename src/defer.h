// clang-format off
#pragma once

#include <assert.h>

#define CONCAT(a, b) a##b
#define CONCAT_EXPAND(a, b) CONCAT(a, b)
#define UNIQUE_NAME(base) CONCAT_EXPAND(base, __LINE__)

//
// clang
//

#if defined(__clang__)
#define defer(block) \
    do { assert(0 && "to be implemented"); } while(0)

// 
// gnu
// 

#elif defined(__GNUC__)
#define defer(block) \
    void UNIQUE_NAME(__cleanup_)(int *ptr __attribute__((unused))) { block } \
    int UNIQUE_NAME(__defer_var_) __attribute__((cleanup(UNIQUE_NAME(__cleanup_)))) = 0

// 
// msvc and other
// 

#else
#define defer(block) \
    do { assert(0 && "only GCC/Clang are supported"); } while(0)
#endif
