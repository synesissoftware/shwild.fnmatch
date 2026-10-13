# shwild.fnmatch - Changes <!-- omit in toc -->


## Unreleased

* Adopted the **misc-dev-scripts** CMake helper gold (SisClr colours; `cmake --build` in place of **MakeCmd**; `MSYSTEM` no longer selects MinGW, use `--mingw`; native `.cmd` runners; **prepare_cmake.sh** retains `--stlsoft-root-dir`);
* **run_all_unit_tests.sh** is now unit-only; **run_all_automated_tests.sh** runs unit then component, **run_all_scratch_tests.sh** no longer runs performance programs, and **run_all_component_tests.sh**, **run_all_performance_tests.sh** and their native `.cmd` counterparts were added;
* **test.scratch.versions** now prints the Phase 3 one-line form (`vJ.N.P.AB (MACRO = 0x........)`), and no longer links **Diagnosticism**, **STLSoft**, or **xTests**; **cstring** and **shwild** remain as its efferent dependencies;
* CI **component tests** step now runs **run_all_component_tests.sh**;


## 0.9.0-rc1 - 18th September 2026

* Recovered the VSS **0.8.2** implementation and landed it as a **CMake** project (version **0.9.0** from historical **0.8.2**);
* Public C API `shwild_fnmatch_*` and C++ `shwild::fnmatch::Pattern` under **include/shwild.fnmatch/**;
* Core library **PRIVATE**-links **cstring** and **shwild**; package config calls `find_dependency()` for both with version floors;
* Core static library includes an empty C++ translation unit so C consumers still link the C++ runtime required by **shwild**;
* Unit tests via **xTests** (with **STLSoft**); examples and scratch **versions** program;
* **test.scratch.versions** uses **Diagnosticism** `calc_version_string()` for rich version output;
* Modular GitHub Actions CI (**ci.yml** + **ci-cell.yml**) with **install-sis-deps** (`STLSoft xTests Diagnosticism cstring shwild`) and install-smoke;
* CMake `find_package` version floors for **cstring**, **shwild**, **Diagnosticism**, **STLSoft**, **xTests**;
* Applied **misc-dev-scripts** **0.6.0** editor/Git/`.sis` drop-in templates on **boilerplate**;
* Restored historical **.gitignore** patterns as a sorted union with **misc-dev-scripts** gold section layout;
* Helper scripts use **collect-c**-style TERM+TTY colour gating;
* README **Dependencies** expressed as **Efferent** / **Afferent**;


<!-- ########################### end of file ########################### -->
