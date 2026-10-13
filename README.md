# shwild.fnmatch <!-- omit in toc -->

**shwild.fnmatch** is a small C and C++ library that provides a platform-independent POSIX `fnmatch()` over **shwild**.


![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=c%2B%2B&logoColor=white)
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![GitHub release](https://img.shields.io/github/v/release/synesissoftware/shwild.fnmatch.svg)](https://github.com/synesissoftware/shwild.fnmatch/releases/latest)
[![Last Commit](https://img.shields.io/github/last-commit/synesissoftware/shwild.fnmatch)](https://github.com/synesissoftware/shwild.fnmatch/commits/master)
[![CI](https://github.com/synesissoftware/shwild.fnmatch/actions/workflows/ci.yml/badge.svg)](https://github.com/synesissoftware/shwild.fnmatch/actions/workflows/ci.yml)


## Table of Contents <!-- omit in toc -->

- [Introduction](#introduction)
- [Installation](#installation)
- [Components](#components)
  - [C API / core library](#c-api--core-library)
  - [C++ API](#c-api)
- [Examples](#examples)
- [Project Information](#project-information)
  - [Where to get help](#where-to-get-help)
  - [Contribution guidelines](#contribution-guidelines)
  - [Dependencies](#dependencies)
    - [Efferent (fan-out)](#efferent-fan-out)
      - [Runtime Dependencies (aka "Normal Dependencies")](#runtime-dependencies-aka-normal-dependencies)
      - [Development Dependencies](#development-dependencies)
    - [Afferent (fan-in)](#afferent-fan-in)
      - [Runtime dependents](#runtime-dependents)
      - [Development dependents](#development-dependents)
  - [Related projects](#related-projects)
  - [License](#license)


## Introduction

**shwild.fnmatch** implements POSIX-style filename matching (`fnmatch()`) on top of **shwild**. The public headers do not include **shwild** or **cstring**; those libraries are implementation dependencies, linked privately.

Flags cover POSIX behaviour (`NOESCAPE`, `PATHNAME`, `PERIOD`) plus Windows-oriented extras (`IGNORECASE` / `NOIGNORECASE`, `NONOESCAPE`, `NOSLASHSWAP`, `NOIMPLICITROOT`).


## Installation

Detailed instructions — via **CMake** — are provided in the accompanying [INSTALL.md](./INSTALL.md) file.


## Components


### C API / core library

The **shwild.fnmatch** C API comprises the following functions:
* `shwild_fnmatch_match()`;
* `shwild_fnmatch_compile_pattern()`;
* `shwild_fnmatch_match_pattern()`;
* `shwild_fnmatch_destroy_pattern()`;

and used as in the following simple example:

```C
#include <shwild.fnmatch/shwild.fnmatch.h>

int r = shwild_fnmatch_match("/usr/*/*", "/usr/include/stlsoft", SHWILD_FNMATCH_F_PATHNAME);
if (SHWILD_FNMATCH_RC_SUCCESS == r) {
    /* matched */
}
```

Compiled patterns are used as follows:

```C
shwild_fnmatch_handle_t hCompiledPattern;
int r2 = shwild_fnmatch_compile_pattern("/usr/*/*", SHWILD_FNMATCH_F_PATHNAME, &hCompiledPattern);
if (r2 < 0) {
    /* report failure */
} else {
    assert(SHWILD_FNMATCH_RC_SUCCESS == shwild_fnmatch_match_pattern(hCompiledPattern, "/usr/include/stlsoft"));
    assert(SHWILD_FNMATCH_RC_NOMATCH == shwild_fnmatch_match_pattern(hCompiledPattern, "/usr/include"));

    shwild_fnmatch_destroy_pattern(hCompiledPattern);
}
```


### C++ API

The C++ API (in **shwild.fnmatch/shwild.fnmatch.hpp**) provides:
* `shwild::fnmatch::match()` — overload of `shwild_fnmatch_match()`;
* `shwild::fnmatch::compile_pattern()` / `match_pattern()` / `destroy_pattern()`;
* `shwild::fnmatch::Pattern` — RAII wrapper around a compiled pattern;
* `shwild::fnmatch::PatternException` — thrown when compile or match fails;

```cpp
#include <shwild.fnmatch/shwild.fnmatch.hpp>

shwild::fnmatch::Pattern pattern("/usr/*/*", SHWILD_FNMATCH_F_PATHNAME);
if (pattern.match("/usr/include/stlsoft")) {
    /* matched */
}
```


## Examples

* **examples/c/example.c.match** — C `shwild_fnmatch_match()` with `SHWILD_FNMATCH_F_PATHNAME`;
* **examples/cpp/example.cpp.pattern** — C++ `shwild::fnmatch::Pattern`;


## Project Information


### Where to get help

[GitHub Issues](https://github.com/synesissoftware/shwild.fnmatch/issues)


### Contribution guidelines

Defect reports, feature requests, and pull requests are welcome on https://github.com/synesissoftware/shwild.fnmatch.


### Dependencies


#### Efferent (fan-out)

Libraries upon which **shwild.fnmatch** depends:


##### Runtime Dependencies (aka "Normal Dependencies")

* [**cstring**](https://github.com/synesissoftware/cstring) — implementation (**PRIVATE**; not on the public include path);
* [**shwild**](https://github.com/synesissoftware/shwild) — implementation (**PRIVATE**; not on the public include path);

Public headers do not include either library. Installed CMake package configuration calls `find_dependency()` for both so a static consumer can link.


##### Development Dependencies

* [**Diagnosticism**](https://github.com/synesissoftware/Diagnosticism) — rich version strings in **test.scratch.versions**;
* [**STLSoft**](https://github.com/synesissoftware/STLSoft) — required by **xTests** public headers (`BUILD_TESTING`);
* [**xTests**](https://github.com/synesissoftware/xTests) — unit-test harness (`BUILD_TESTING`);


#### Afferent (fan-in)

Projects that depend on **shwild.fnmatch**:


##### Runtime dependents

* \<none>;


##### Development dependents

* \<none>;


### Related projects

* [**cstring**](https://github.com/synesissoftware/cstring) — extensible C-style strings;
* [**Diagnosticism**](https://github.com/synesissoftware/Diagnosticism) — diagnostics utilities, including version-string formatting;
* POSIX [`fnmatch()`](https://pubs.opengroup.org/onlinepubs/9699919799/functions/fnmatch.html);
* [**shwild**](https://github.com/synesissoftware/shwild) — shell-compatible wildcard matching;


### License

**shwild.fnmatch** is released under the 3-clause BSD license. See [LICENSE](./LICENSE) for details.


<!-- ########################### end of file ########################### -->
