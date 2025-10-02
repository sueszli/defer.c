      ▄▄               ▄▄▄▄                                          
      ██              ██▀▀▀                                          
 ▄███▄██   ▄████▄   ███████    ▄████▄    ██▄████             ▄█████▄ 
██▀  ▀██  ██▄▄▄▄██    ██      ██▄▄▄▄██   ██▀                ██▀    ▀ 
██    ██  ██▀▀▀▀▀▀    ██      ██▀▀▀▀▀▀   ██                 ██       
▀██▄▄███  ▀██▄▄▄▄█    ██      ▀██▄▄▄▄█   ██          ██     ▀██▄▄▄▄█ 
  ▀▀▀ ▀▀    ▀▀▀▀▀     ▀▀        ▀▀▀▀▀    ▀▀          ▀▀       ▀▀▀▀▀  

a tiny, portable, header-only C library for a scope-based cleanup (RAII-like semantics).

works both with gcc and clang (with `BlocksRuntime` on linux).

# demo

```
$ tail -n +6 src/main.c

    int main(void) {
        char *mem = malloc(100);
        defer({
            free(mem);
            printf("mem freed\n");
        });

        printf("mem allocated\n");
        return EXIT_SUCCESS;
    }

$ make run

    mem allocated
    mem freed

$ make docker-gcc-test

    100% tests passed, 0 tests failed out of 1

$ make docker-clang-test

    100% tests passed, 0 tests failed out of 1

# limitations

(1) you can only have one defer block per line, because the `UNIQUE_NAME` macro uses `__LINE__` for name generation.

```
BAD:  defer({ cleanup_a(); }); defer({ cleanup_b(); });
GOOD: defer({ cleanup_a(); });
      defer({ cleanup_b(); });
```

(2) clang requires the `__block` qualifier for mutable variables in defer blocks.

```
GOOD: __block int counter = 0;  // required for clang
      defer({ counter++; });
```

(3) clang can't capture arrays in defer blocks, even with `__block`. use pointers instead.

```
BAD:  int arr[3] = {1, 2, 3};
      defer({ arr[0] = 0; });

GOOD: int arr[3] = {1, 2, 3};
      int *p = arr;            // required for clang
      defer({ p[0] = 0; });
```

(4) the name `ptr` is reserved.

(5) comma operators are interpreted as macros. use semicolons instead.

```
BAD:  defer({ a = 1, b = 2; });      // ERROR: comma seen as separator
GOOD: defer({ a = 1; b = 2; });      // OK: semicolon separates statements
```

(6) `setjmp`/`longjmp` is not supported.

```
BAD: int *data = malloc(100);
     defer({ free(data); });

     if (setjmp(buf)) {
         return; // jumped here, defer didn't execute, data is leaked
     }
```

(7) signal handlers (signals, abort, _exit) are not supported.

(8) `goto` statements behave counterintuitively. jumping OVER a defer block means it never executes. jumping OUT OF a defer scope will execute the defer (as expected).

```
BAD: goto skip;
     defer({ cleanup(); });  // skipped, never executes
     skip:
```

(9) defer blocks are executed in LIFO order (matching Go's defer behavior). defer executes AFTER return value is computed but BEFORE function exits.

```
GOOD: defer({ printf("1"); });
      defer({ printf("2"); });
      defer({ printf("3"); });
      // prints: "321"
```

(10) defer scopes are created by curly braces `{}`. each scope has its own defer blocks. this includes function bodies, loops, and conditional blocks.

(11) variables captured by defer block must outlive the defer's execution.

# references

interesting reads and alternative implementations, each with their own caveats:

- https://thephd.dev/_vendor/future_cxx/technical%20specification/C%20-%20defer/C%20-%20defer%20Technical%20Specification.pdf
- https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/cleanup.h
- https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2895.htm
- https://thephd.dev/c2y-the-defer-technical-specification-its-time-go-go-go
- https://fdiv.net/2015/10/08/emulating-defer-c-clang-or-gccblocks
- https://gist.github.com/eloraiby/f64fcba0d489f0d31aa544d66cbfd7a6
