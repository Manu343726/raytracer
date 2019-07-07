# Raytracer

My implementation of **"Raytracing In One Weekend"** by Peter Shirley.

See [this post](https://manu343726.github.io/2019-06-20-raytracer-runtime-postmortem/) for details.

# Build and run

The project uses [conan](https://conan.io) for dependencies and CMake as
buildsystem. It's been tested on Linux only. Requires C++17. To build the
project run:

``` shell
$ mkdir build && cd build
$ conan install .. --build=missing -g cmake
$ cmake ..
$ make -j$(nproc --all)
```

# License

Everything published in this repo is licensed under MIT license.
