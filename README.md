Vaslet is a lightweight JSON configuration service for low-cost embedded Linux
devices. It provides a server, a client shared library with a public API, and a
command-line utility, with a target combined binary size of less than 100 KB.

> **Vaslet is in early development.** The features below describe the intended
> scope. No usable release or size measurements are available yet.

English | [简体中文](docs/README_zh.md)

# Components

| Component | Name | Purpose |
| --- | --- | --- |
| Server | `vasletd` | Background configuration service; `d` stands for daemon |
| Client shared library | `libvaslet` | Produces `libvaslet.so` on Linux |
| Command-line utility | `vasletctl` | Query, modify, and synchronize configuration (planned) |

# Features

Planned features:

* Configuration data is represented as JSON.
* A dedicated server manages configuration for applications on the device.
* Applications access configuration through a client shared library and public API.
* A command-line utility provides configuration access for debugging and scripts.
* The server, client shared library, and command-line utility together target a
  compiled size of less than 100 KB.
* Dependencies and runtime resource usage are kept small for embedded devices.

# Documentation

The [Chinese README](docs/README_zh.md) is bundled with the source code.
See the [API notes](docs/api.md), [command-line reference](docs/cli.md), and
[build instructions](docs/building.md) (currently in Chinese).

# Repository Contents

The planned layout separates public API headers (`include/vaslet/`), component
implementations (`src/server/`, `src/client/`, and `src/cli/`), internal shared
code (`src/common/`), tests, examples, and build tools. See the
[directory plan](docs/project-layout.md) (in Chinese) for component boundaries
and delivery targets. The initial buildable skeleton is now in place.

# Limitations

* The target platform is embedded Linux. Support for other operating systems has
  not been defined.
* Only a buildable skeleton is available: version queries and help output work,
  but configuration access and communication are not implemented. Configuration
  API signatures and command syntax are not finalized.
* The 100 KB size target has not been verified. Runtime memory usage must be
  measured separately.

# Getting the Source

```sh
git clone https://github.com/ChnjFan/Vaslet.git
```

# Building

The skeleton uses CMake 3.20 or newer, C++17, with no third-party dependencies.

```sh
cmake -S . -B build/host-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/host-debug
ctest --test-dir build/host-debug --output-on-failure
```

See [building and installation](docs/building.md) for examples, release builds,
and cross-compilation notes. Configuration operations are not available yet.


GitHub Actions builds and tests Debug and Release on Linux with GCC and Clang.
Release runs upload installation archives and size reports; see the
[CI instructions](docs/building.md#github-actions) for details.

# Contributing

C++ code follows Google C++ style, configured in [`.clang-format`](.clang-format).
To format a source file, run from the repository root:

```sh
clang-format -i --style=file path/to/file.cpp
```

# Binary Size

The size budget covers the server executable, client shared library, and
command-line executable combined.

Future reports will specify the target architecture, compiler, optimization
options, symbol stripping, and additional runtime dependencies. The byte
definition of KB and the treatment of dependencies will be established with the
build and measurement process. No size or performance results are available yet.

# License

Vaslet is licensed under the [MIT License](LICENSE).
