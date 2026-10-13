# shwild.fnmatch - Installation and Use <!-- omit in toc -->

**shwild.fnmatch** is a classic-form C/C++ library, insofar as it has
implementation files in its **src** directory and header files in its
**include/shwild.fnmatch** directory. Thus, once "installed", one must
simply include **shwild.fnmatch/shwild.fnmatch.h** (or
**shwild.fnmatch/shwild.fnmatch.hpp**), and compile-in or link-in the
implementation.

The implementation requires installed **cstring** and **shwild** CMake
packages. When testing is enabled, **STLSoft**, **xTests**, and
**Diagnosticism** must also be findable (**Diagnosticism** is used by
**test.scratch.versions** for rich version strings).


## Table of Contents <!-- omit in toc -->

- [CMake](#cmake)
- [Bundled](#bundled)


## CMake

The primary choice for installation is by use of **CMake**.

1. Obtain the latest distribution of **shwild.fnmatch**, from
   https://github.com/synesissoftware/shwild.fnmatch/, e.g.

   ```bash
   $ mkdir -p ~/open-source
   $ cd ~/open-source
   $ git clone https://github.com/synesissoftware/shwild.fnmatch/
   ```

2. Install **cstring** and **shwild** so that `find_package(cstring)` and
   `find_package(shwild)` succeed. Point **CMake** at that prefix via
   `CMAKE_PREFIX_PATH` (or an equivalent toolchain/package config path).
   When building tests, also install **STLSoft**, **xTests**, and
   **Diagnosticism** under the same prefix (or otherwise make them
   findable).

3. Prepare the CMake configuration, via the **prepare_cmake.sh** script, as
   in:

   ```bash
   $ cd ~/open-source/shwild.fnmatch
   $ CMAKE_PREFIX_PATH=/path/to/sis-deps ./prepare_cmake.sh -v
   ```

   Useful optional flags:

   * `--disable-examples` / `-E` — omit examples (`BUILD_EXAMPLES=OFF`);
   * `--disable-testing` / `-T` — omit tests (`BUILD_TESTING=OFF`);
   * `--stlsoft-root-dir` / `-s` — pass an explicit **STLSoft** root as
     the CMake variable **STLSOFT**;

   (**Hint**: execute `$ ./prepare_cmake.sh --help` for more information.)

4. Run a build of the generated **CMake**-derived build files via the
   **build_cmake.sh** script, as in:

   ```bash
   $ ./build_cmake.sh
   ```

   (**NOTE**: if you provide the flag `--run-make` (=== `-m`) in step 3 then
   you do not need this step.)

5. As a check (when testing was not disabled), execute the built unit-test
   programs via **run_all_unit_tests.sh**, as in:

   ```bash
   $ ./run_all_unit_tests.sh
   ```

6. Install the library on the host, via `cmake`, as in:

   ```bash
   $ cmake --install ${SIS_CMAKE_BUILD_DIR:-./_build} --config Release
   ```

7. Then to use the library, it is a simple matter as follows:

   1. Assuming a simplest possible program to verify the installation:

      ```c
      /* main.c */
      #include <shwild.fnmatch/shwild.fnmatch.h>

      int main(void)
      {
        return 0 == shwild_fnmatch_match("a*", "abc", 0) ? 0 : 1;
      }
      ```

   2. Prefer the CMake package:

      ```cmake
      find_package(shwild.fnmatch REQUIRED)
      target_link_libraries(app PRIVATE shwild.fnmatch::core)
      ```

      Enable **C++** in the consuming **CMake** project (`project(... C CXX)`
      or equivalent) even when the application sources are C: **shwild** is
      implemented in C++, so the C++ runtime must be on the link line.

      The package configuration re-finds **cstring** and **shwild**, so
      their prefixes must still be on `CMAKE_PREFIX_PATH`.


## Bundled

When embedding the sources, compile **src/shwild.fnmatch.c** and add
**include/** to the include path. Link **cstring** and **shwild**. Do not
add **cstring** or **shwild** headers to application translation units
unless those APIs are used directly.


<!-- ########################### end of file ########################### -->
