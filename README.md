# CS3203 SPA
## Prerequisites 
This project is known to work with
1. `cmake 3.13.3`
2. `qt 5.12`

This project is **incompatible** with GCC8

## Building this project
1. Create a build directory with `mkdir build`
2. Enter the build directory with `cd build`
3. Run `CXX=clang++ cmake ..` in the build directory. 
    - For Qt installations that are not automatically found (e.g. you receive errors during `make` or `cmake` related to `ui_mainwindow.h`, etc)
        - Run `cmake -DCMAKE_PREFIX_PATH=<path to Qt prefix for cmake> .`  instead. You may have to delete and re-clone this project before doing so.
        - A specific run could be `cmake -DCMAKE_PREFIX_PATH=/opt/Qt5.12.0/5.12.0/gcc_64/lib/cmake .` when the cmake prefix for the Qt installation is in `/opt/Qt5.12.0/5.12.0/gcc_64/lib/cmake`
    - For using a particular compiler like `clang`, two environment variables need to be set
        - `CC=clang CXX=clang++ ...`
        - Combining this with the command above:  
            - `CC=clang CXX=clang++ cmake -DCMAKE_PREFIX_PATH=/opt/Qt5.12.0/5.12.0/gcc_64/lib/cmake .`
    - To compile `autotester_gui`, add the flag `-DBUILD_GUI=ON` to your `cmake` invocation.
4. Run `make` in the build directory. The build can be parallelized with `make -j<num>`, e.g. `make -j4`.

## Contributing Guidelines
Please read [the Contribution Guidelines](./CONTRIBUTING.md) when formatting your pull request.

## Running System Tests With Autotester
`runtests.sh` is included in the root folder to run the various system tests through autotester. 
The autotester output will be created in `tests/system_tests/output`.
You might need to use chmod to change access permission of `runtests.sh` 

Paste `analysis.xsl` from `tests/` into `tests/system_tests/output` and then open the individual test files in Firefox to view the test output graphically. Chrome won't allow you to do so. 

Alternatively:
`grep -B 14 -n -r /failed tests/system_tests/output/` will check for failed cases and output the 14 lines before each failed case to see the context surrounding it. 

This can be chained into more complex calls: to check for any tests that failed that don't have the word "Pattern": 

`grep -B 14 -n -r /failed tests/system_tests/output/ | grep id | grep -v Pattern`


## Generating documentation
1. Run `doxygen` in the root project folder. 
2. Documentation should be generated in `doxygen-docs`.
3. `doxygen-docs/index.html` (open in browser) will lead to the documentation homepage.
