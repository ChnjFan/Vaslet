# Vaslet

Vaslet is a Linux-focused C++ project for a lightweight JSON configuration
center.

## Quick Start

Requirements: Linux, Git, CMake 3.20 or later, a C++17 compiler (GCC or Clang),
and a build tool such as Make or Ninja.

RapidJSON is included as a Git submodule under `thirdparty/rapidjson`. The
`vasletd` target includes headers from `thirdparty/rapidjson/include`.

After cloning the repository, initialize the dependency from the project root:

```sh
git submodule update --init thirdparty/rapidjson
```

From the project root, initialize the dependency and build the Release target:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target vasletd --parallel
```

The configured executable output path is `build/bin/vasletd`.

## License

Vaslet is licensed under the [MIT License](LICENSE).
RapidJSON's license and bundled third-party notices are available in
[thirdparty/rapidjson/license.txt](thirdparty/rapidjson/license.txt).

