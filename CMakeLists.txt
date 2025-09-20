cmake_minimum_required(VERSION 3.27 FATAL_ERROR)
project(main LANGUAGES C)

if(CMAKE_C_COMPILER_ID MATCHES "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS 9.3)
    message(FATAL_ERROR "GCC is too old: ${CMAKE_C_COMPILER_VERSION}")
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang" AND CMAKE_C_COMPILER_VERSION VERSION_LESS 10.0)
    message(FATAL_ERROR "Clang is too old: ${CMAKE_C_COMPILER_VERSION}")
endif()

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# https://github.com/ossf/wg-best-practices-os-developers/blob/main/docs/Compiler-Hardening-Guides/Compiler-Options-Hardening-Guide-for-C-and-C%2B%2B.md
# https://github.com/airbus-seclab/c-compiler-security

# setting flags correctly in cmake: https://github.com/pytorch/pytorch/blob/264e7f68a095b5b2f10e7ca66a3bbb16f6e56e28/CMakeLists.txt#L1043

# both: -O2 -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough -Werror=format-security  -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3  -D_GLIBCXX_ASSERTIONS  -fstrict-flex-arrays=3  -fstack-clash-protection -fstack-protector-strong  -Wl,-z,nodlopen -Wl,-z,noexecstack  -Wl,-z,relro -Wl,-z,now  -Wl,--as-needed -Wl,--no-copy-dt-needed-entries

# gcc: -Wtrampolines -fzero-init-padding-bits=all -Wbidi-chars=any



add_compile_definitions(DEBUG)
remove_compile_definitions(NDEBUG)
