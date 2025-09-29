> See: https://github.com/ossf/wg-best-practices-os-developers/blob/main/docs/Compiler-Hardening-Guides/Compiler-Options-Hardening-Guide-for-C-and-C%2B%2B.md

When compiling C or C++ code on compilers such as GCC and clang, turn on these flags in all cases for detecting vulnerabilities at compile time and enable run-time protection mechanisms:

```sh
-O2 -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough \
-Werror=format-security \
-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 \
-D_GLIBCXX_ASSERTIONS \
-fstrict-flex-arrays=3 \
-fstack-clash-protection -fstack-protector-strong \
-Wl,-z,nodlopen -Wl,-z,noexecstack \
-Wl,-z,relro -Wl,-z,now \
-Wl,--as-needed -Wl,--no-copy-dt-needed-entries
```

In addition, when compiling code in any of the situations in the below table, add the corresponding additional options:

| When                                                    | Additional options flags                                                                                 |
|:------------------------------------------------------- |:---------------------------------------------------------------------------------------------------------|
| using GCC                                               | `-Wtrampolines -fzero-init-padding-bits=all`                                                             |
| using GCC and only left-to-right writing in source code | `-Wbidi-chars=any`                                                                                       |
| for executables                                         | `-fPIE -pie`                                                                                             |
| for shared libraries                                    | `-fPIC -shared`                                                                                          |
| for x86_64                                              | `-fcf-protection=full`                                                                                   |
| for aarch64                                             | `-mbranch-protection=standard`                                                                           |
| for production code                                     | `-fno-delete-null-pointer-checks -fno-strict-overflow -fno-strict-aliasing -ftrivial-auto-var-init=zero` |
| for C code treating obsolete C constructs as errors     | `-Werror=implicit -Werror=incompatible-pointer-types -Werror=int-conversion`                             |
| for multi-threaded C code using GNU C library pthreads  | `-fexceptions`                                                                                           |
| during development but *not* when distributing source   | `-Werror`                                                                                                |

Table 1: Recommended compiler options that enable strictly compile-time checks.

| Compiler Flag                                                                 |       Supported since       | Description                                                                         |
|:----------------------------------------------------------------------------- |:------------------------:|:----------------------------------------------------------------------------------- |
| `-Wall`<br/>`-Wextra`                                   | GCC 2.95.3<br/>Clang 4.0.0 | Enable warnings for constructs often associated with defects                        |
| `-Wformat`<br/>`-Wformat=2`                       | GCC 2.95.3<br/>Clang 4.0.0 | Enable additional format function warnings                                          |
| `-Wconversion`<br/>`-Wsign-conversion` | GCC 2.95.3<br/>Clang 4.0.0 | Enable implicit conversion warnings                                                 |
| `-Wtrampolines`                                             |         GCC 4.3.0          | Enable warnings about trampolines that require executable stacks                    |
| `-Wimplicit-fallthrough`                           |         GCC 7.0.0<br>Clang 4.0.0   | Warn when a switch case falls through                                           |
| `-Wbidi-chars=any`                                       | GCC 12.0.0                   | Enable warnings for possibly misleading Unicode bidirectional control characters    |
| `-Werror`<br/>`-Werror=`*`<warning-flag>`*       | GCC 2.95.3<br/>Clang 2.6.0 | Treat all or selected compiler warnings as errors. Use the blanket form `-Werror` only during development, not in source distribution. |
| `-Werror=format-security`                         | GCC 2.95.3<br/>Clang 4.0.0 | Treat format strings that are not string literals and used without arguments as errors                                                 |
| `-Werror=implicit`<br/>`-Werror=incompatible-pointer-types`<br/>`-Werror=int-conversion`<br/> | GCC 2.95.3<br/>Clang 2.6.0 | Treat obsolete C constructs as errors |

Table 2: Recommended compiler options that enable run-time protection mechanisms.

| Compiler Flag                                                                             |            Supported since            | Description                                                                                  |
|:----------------------------------------------------------------------------------------- |:----------------------------------:|:-------------------------------------------------------------------------------------------- |
| `-D_FORTIFY_SOURCE=3`| GCC 12.0.0<br/>Clang 9.0.0  | Fortify sources with compile- and run-time checks for unsafe libc usage and buffer overflows. Some fortification levels can impact performance. Requires `-O1` or higher, may require prepending `-U_FORTIFY_SOURCE`. |
| `-D_GLIBCXX_ASSERTIONS` | libstdc++ 6.0.0  | Precondition checks for C++ standard library calls. Can impact performance.                  |
| `-fstrict-flex-arrays=3`                             |       GCC 13.0.0<br/>Clang 16.0.0       | Consider a trailing array in a struct as a flexible array if declared as `[]`                           |
| `-fstack-clash-protection`                                   |       GCC 8.0.0<br/>Clang 11.0.0       | Enable run-time checks for variable-size stack allocation validity. Can impact performance.  |
| `-fstack-protector-strong`                                   | GCC 4.9.0<br/>Clang 6.0.0          | Enable run-time checks for stack-based buffer overflows. Can impact performance.             |
| `-fcf-protection=full`                                           | GCC 8.0.0<br/>Clang 7.0.0          | Enable control-flow protection against return-oriented programming (ROP) and jump-oriented programming (JOP) attacks on x86_64 |
| `-mbranch-protection=standard`                           | GCC 9.0.0<br/>Clang 8.0.0          | Enable branch protection against ROP and JOP attacks on AArch64 |
| `-Wl,-z,nodlopen` |           Binutils 2.10.0            | Restrict `dlopen(3)` calls to shared objects                                 |
| `-Wl,-z,noexecstack`                                               |           Binutils 2.14.0            | Enable data execution prevention by marking stack memory as non-executable                   |
| `-Wl,-z,relro`<br/>`-Wl,-z,now`                           |           Binutils 2.15.0            | Mark relocation table entries resolved at load-time as read-only. `-Wl,-z,now` can impact startup performance.                            |
| `-fPIE -pie`                                                               |   Binutils 2.16.0<br/>Clang 5.0.0    | Build as position-independent executable. Can impact performance on 32-bit architectures.                                                   |
| `-fPIC -shared`                                                         | < Binutils 2.6.0<br/>Clang 5.0.0     | Build as position-independent code. Can impact performance on 32-bit architectures.                                                         |
| `-fno-delete-null-pointer-checks`                     | GCC 3.0.0<br/>Clang 7.0.0            | Force retention of null pointer checks                                                       |
| `-fno-strict-overflow`                                           | GCC 4.2.0                            | Define behavior for signed integer and pointer arithmetic overflows                        |
| `-fno-strict-aliasing`                                           | GCC 2.95.3<br/>Clang 2.9.0        | Do not assume strict aliasing                                                                |
| `-ftrivial-auto-var-init`                                     | GCC 12.0.0<br/>Clang 8.0.0               | Perform trivial auto variable initialization                                                 |
| `-fexceptions`                                                           | GCC 2.95.3<br/>Clang 2.6.0           | Enable exception propagation to harden multi-threaded C code                                 |
| `-fhardened`                                                               | GCC 14.0.0                           | Enable pre-determined set of hardening options in GCC                                        |
| `-Wl,--as-needed`<br/>`-Wl,--no-copy-dt-needed-entries` | Binutils 2.20.0 | Allow linker to omit libraries specified on the command line to link against if they are not used |
| `-fzero-init-padding-bits=all`                           | GCC 15.0.0                            | Guarantee zero initialization of padding bits in all automatic variable initializers |
