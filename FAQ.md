# shwild.fnmatch - FAQ <!-- omit in toc -->

The FAQ list is under (constant) development. If you post a question on the
Issues forum (https://github.com/synesissoftware/shwild.fnmatch/issues)
it will be used to create one.


## Table of Contents <!-- omit in toc -->

- [Q1: "How do I build shwild.fnmatch?"](#q1-how-do-i-build-shwildfnmatch)
- [Q2: "How do I install shwild.fnmatch?"](#q2-how-do-i-install-shwildfnmatch)
- [Q3: "How do I use shwild.fnmatch?"](#q3-how-do-i-use-shwildfnmatch)
- [Q4: "Does shwild.fnmatch have its own unit-tests?"](#q4-does-shwildfnmatch-have-its-own-unit-tests)
- [Q5: "What are the dependencies?"](#q5-what-are-the-dependencies)
- [Q6: "How does this differ from POSIX fnmatch()?"](#q6-how-does-this-differ-from-posix-fnmatch)


# FAQs: <!-- omit in toc -->


## Q1: "How do I build shwild.fnmatch?"

See [INSTALL.md](./INSTALL.md) for the recommended **CMake** flow
(**prepare_cmake.sh**, then **build_cmake.sh**).

Install **cstring** and **shwild** first, then:

```bash
$ CMAKE_PREFIX_PATH=/path/to/sis-deps ./prepare_cmake.sh -m
```

Execute `$ ./prepare_cmake.sh --help` for the full set of options.


## Q2: "How do I install shwild.fnmatch?"

See [INSTALL.md](./INSTALL.md) for details of how to install
**shwild.fnmatch**.


## Q3: "How do I use shwild.fnmatch?"

Include **shwild.fnmatch/shwild.fnmatch.h** (or
**shwild.fnmatch/shwild.fnmatch.hpp**) and link against
`shwild.fnmatch::core` (the installed library name is
**shwild.fnmatch.core**).

A minimal sketch:

```c
#include <shwild.fnmatch/shwild.fnmatch.h>

int main(void)
{
    return 0 == shwild_fnmatch_match("a*", "abc", 0) ? 0 : 1;
}
```

See [INSTALL.md](./INSTALL.md) and the examples under **examples/**.


## Q4: "Does shwild.fnmatch have its own unit-tests?"

Yes. Automated unit tests live under **test/unit/** and use **xTests**.
After a successful configure/build:

```bash
$ ./run_all_unit_tests.sh
```


## Q5: "What are the dependencies?"

The implementation **PRIVATE**-links **cstring** and **shwild**. Public
headers do not include either. Tests require **STLSoft** and **xTests**
(when `BUILD_TESTING` is on). The scratch **versions** program
additionally requires **Diagnosticism** for rich version-string
formatting. Because **shwild** is C++, a C application must still link
with a C++ compiler/runtime (CMake: enable **CXX**).


## Q6: "How does this differ from POSIX fnmatch()?"

The API is POSIX-inspired (`NOESCAPE`, `PATHNAME`, `PERIOD`) and adds
Windows-oriented flags (`IGNORECASE` / `NOIGNORECASE`, `NONOESCAPE`,
`NOSLASHSWAP`, `NOIMPLICITROOT`). Matching is implemented over **shwild**
rather than a libc `fnmatch()`.


<!-- ########################### end of file ########################### -->
