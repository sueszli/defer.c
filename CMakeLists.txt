project(demo LANGUAGES C)

#
# compiler hardening
# https://github.com/ossf/wg-best-practices-os-developers/blob/main/docs/Compiler-Hardening-Guides/Compiler-Options-Hardening-Guide-for-C-and-C%2B%2B.md
#

message(STATUS "make version: ${CMAKE_VERSION}")
message(STATUS "compiler version: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "architecture: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "system: ${CMAKE_SYSTEM_NAME}")

# minimum versions
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)
if(CMAKE_C_COMPILER_ID MATCHES "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS 13.0.0)
  message(FATAL_ERROR "GCC is outdated: ${CMAKE_C_COMPILER_VERSION}")
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang" AND CMAKE_C_COMPILER_VERSION VERSION_LESS 16.0.0)
  message(FATAL_ERROR "Clang is outdated: ${CMAKE_C_COMPILER_VERSION}")
endif()

# default options
add_compile_options(
  -O2
  -Wall
  -Wformat
  -Wformat=2
  -Wconversion
  -Wimplicit-fallthrough
  -Werror=format-security
  -U_FORTIFY_SOURCE
  -D_FORTIFY_SOURCE=3
  -D_GLIBCXX_ASSERTIONS
  -fstrict-flex-arrays=3
  -fstack-clash-protection
  -fstack-protector-strong
  -Wl,-z,nodlopen
  -Wl,-z,noexecstack
  -Wl,-z,relro
  -Wl,-z,now
  -Wl,--as-needed
  -Wl,--no-copy-dt-needed-entries
  # deprecated c calls
  -Werror=implicit
  -Werror=incompatible-pointer-types
  -Werror=int-conversion
  # multithreaded with pthreads
  -fexceptions
  # other compile time options (table 1 from ossf doc)
  -Wall
  -Wextra
  -Wsign-conversion
  -Wtrampolines
  -Wbidi-chars=any
  # executable (for shared libraries use `-fPIC -shared`)
  -fPIE
  -pie
)

# debug and release mode options
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_BUILD_TYPE STREQUAL "Release")
  message(FATAL_ERROR "Cannot build in both Debug and Release mode simultaneously")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-Werror)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_options(
    -fno-delete-null-pointer-checks -fno-strict-overflow -fno-strict-aliasing -ftrivial-auto-var-init=zero
  )
endif()

# compiler specific options
if(CMAKE_C_COMPILER_ID MATCHES "GNU")
  add_compile_options(-Wtrampolines -fzero-init-padding-bits=all -Wbidi-chars=any -fhardened)
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang")

endif()

# architecture specific options
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|i686|i386")
  add_compile_options(-fcf-protection=full)
endif()
if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64")
  add_compile_options(-mbranch-protection=standard)
endif()

#
# sources
#

add_executable(demo demo.c)
