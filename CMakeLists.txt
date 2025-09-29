project(main LANGUAGES C)

# 
# compiler hardening
# see: https://github.com/ossf/wg-best-practices-os-developers/blob/main/docs/Compiler-Hardening-Guides/Compiler-Options-Hardening-Guide-for-C-and-C%2B%2B.md
# 

message(STATUS "detected make version: ${CMAKE_VERSION}")
message(STATUS "detected compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "detected architecture: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "detected system: ${CMAKE_SYSTEM_NAME}")

# c standard version
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# minimum make version
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

# minimum gcc version
if(CMAKE_C_COMPILER_ID MATCHES "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS 13.0.0)
  message(FATAL_ERROR "GCC is too old: ${CMAKE_C_COMPILER_VERSION}")
endif()

# minimum clang version
if(CMAKE_C_COMPILER_ID MATCHES "Clang" AND CMAKE_C_COMPILER_VERSION VERSION_LESS 16.0.0)
  message(FATAL_ERROR "Clang is too old: ${CMAKE_C_COMPILER_VERSION}")
endif()

add_compile_options(
    # default default options
    -O2 -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough
    -Werror=format-security
    -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3
    -D_GLIBCXX_ASSERTIONS
    -fstrict-flex-arrays=3
    -fstack-clash-protection -fstack-protector-strong
    -Wl,-z,nodlopen -Wl,-z,noexecstack
    -Wl,-z,relro -Wl,-z,now
    -Wl,--as-needed -Wl,--no-copy-dt-needed-entries
    # deprecated c calls
    -Werror=implicit -Werror=incompatible-pointer-types -Werror=int-conversion
    # multithreaded with pthreads
    -fexceptions
    # other compile time options (table 1 from ossf doc)
    -Wall
    -Wextra
    -Wsign-conversion
    -Wtrampolines
    -Wbidi-chars=any
)

# TODO: ....

# development mode but not release
#   -Werror

# release mode but not production
#   -fno-delete-null-pointer-checks -fno-strict-overflow -fno-strict-aliasing -ftrivial-auto-var-init=zero

# for executables: -> set with global flag
#   -fPIE -pie

# for shared libraries: -> set with global flag
#   -fPIC -shared


# gcc only options
if(CMAKE_C_COMPILER_ID MATCHES "GNU")
  add_compile_options(
    -Wtrampolines -fzero-init-padding-bits=all
    -Wbidi-chars=any
    -fhardened
  )
endif()

# clang only options
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    # ...
endif()

# x86 only options
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|i686|i386")
  add_compile_options(
    -fcf-protection=full
  )
endif()

# aarch64 only options
if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64")
  add_compile_options(
    -mbranch-protection=standard
  )
endif()
