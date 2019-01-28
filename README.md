# CS3203 SPA
## Prerequisites 
This project is known to work with
1. `cmake 3.13.3`
2. `qt 5.12`

## Building this project
1. Create a build directory with `mkdir build`
2. Enter the build directory with `cd build`
3. Run `cmake ..` in the build directory. 
    - For Qt installations that are not automatically found (e.g. you receive errors during `make` or `cmake` related to `ui_mainwindow.h`, etc)
        - Run `cmake -DCMAKE_PREFIX_PATH=<path to Qt prefix for cmake> .`  instead. You may have to delete and re-clone this project before doing so.
        - A specific run could be `cmake -DCMAKE_PREFIX_PATH=/opt/Qt5.12.0/5.12.0/gcc_64/lib/cmake .` when the cmake prefix for the Qt installation is in `/opt/Qt5.12.0/5.12.0/gcc_64/lib/cmake`
    - For using a particular compiler like `clang`, two environment variables need to be set
        - `CC=clang CXX=clang++ ...`
        - Combining this with the command above:  
            - `CC=clang CXX=clang++ cmake -DCMAKE_PREFIX_PATH=/opt/Qt5.12.0/5.12.0/gcc_64/lib/cmake .`
4. Run `make` in the build directory. The build can be parallelized with `make -j<num>`, e.g. `make -j4`.

## Contributing Guidelines
Please read [the Contribution Guidelines](./CONTRIBUTING.md) when formatting your pull request.