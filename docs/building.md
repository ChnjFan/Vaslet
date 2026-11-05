# 构建与安装

需要 CMake 3.20 或更新版本，以及支持 C++17 的编译器。正式交付产物无第三方依赖；
单元测试使用固定版本的 [GoogleTest v1.17.0](https://github.com/google/googletest/tree/v1.17.0)，
采用 [BSD-3-Clause 许可证](https://github.com/google/googletest/blob/v1.17.0/LICENSE)。
开启测试时，上游 GoogleTest 的 CMake 配置还会探测 C 编译器；项目和测试源码仍为 C++。
正式部署目标为 Linux；macOS 可用于本机骨架验证，动态库后缀为 `.dylib`。

## 本机开发

在仓库根目录执行：

```sh
cmake -S . -B build/host-debug -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON -DVASLET_BUILD_EXAMPLES=ON
cmake --build build/host-debug
ctest --test-dir build/host-debug --output-on-failure
./build/host-debug/bin/vaslet_client_example
```

构建文件参考 Jianm 的分区风格：根目录负责全局设置、构建开关、输出目录、告警策略和安装规则，
`src/CMakeLists.txt` 按模块目录集中收集源码并定义交付目标，`tests/CMakeLists.txt` 集中定义测试目标。
源码通过 `file(GLOB_RECURSE ... CONFIGURE_DEPENDS <模块目录>/*.cpp <模块目录>/*.h)` 收集，
新增源码会自动进入所属模块，不逐个列出源码文件名。

| 选项 | 默认值 | 用途 |
| --- | --- | --- |
| `CMAKE_BUILD_TYPE` | `Release` | 单配置生成器的构建类型 |
| `BUILD_TESTING` | `ON` | 构建并注册 GoogleTest 单元测试及集成检查 |
| `VASLET_BUILD_EXAMPLES` | `OFF` | 构建客户端示例 |

项目目标启用严格告警：GCC/Clang 使用 `-Wall -Wextra -Wpedantic -Werror`，MSVC 使用 `/W4 /WX`。
未安装的程序、测试和示例统一位于构建目录的 `bin/`，动态库和内部静态库位于 `lib/`。
例如服务端为 `build/host-debug/bin/vasletd`，客户端库在 macOS 为 `build/host-debug/lib/libvaslet.dylib`。
多配置生成器通过 `cmake --build <构建目录> --config Debug` 选择配置，产物位于 `bin/Debug/` 和 `lib/Debug/`。

测试覆盖 C++ 调用方的编译链接、开关和字符串值解析与状态重置、帮助与版本输出，以及无效参数和未实现操作的错误退出。
尚无配置读写测试。版本接口示例不需要启动服务端。

## 测试开关与依赖

`BUILD_TESTING` 是项目测试构建开关，默认 `ON`，开启时调用 `enable_testing()`。单元测试使用 GoogleTest 的断言和
`gtest_main`，由 `gtest_discover_tests` 在运行 CTest 前发现测试；命令行集成检查仍使用 CMake 脚本。
首次开启测试时，FetchContent 下载 v1.17.0 源码包并校验 SHA-256；下载及构建产物位于 `build/` 下。
GoogleTest 仅静态链接到测试程序，关闭其安装规则，不进入交付产物或发布安装目录。

只构建交付产物时使用：

```sh
cmake -S . -B build/no-tests -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
cmake --build build/no-tests
```

关闭测试时不下载、不配置或编译 GoogleTest，也不构建单元测试或注册集成测试。
如需离线开启测试，可预先将官方 v1.17.0 源码解压到 `build/dependencies/googletest-1.17.0/`，再运行：

```sh
cmake -S . -B build/host-debug -DBUILD_TESTING=ON \
  -DFETCHCONTENT_SOURCE_DIR_GOOGLETEST="$PWD/build/dependencies/googletest-1.17.0"
cmake --build build/host-debug
ctest --test-dir build/host-debug --output-on-failure
```

## 发布安装

```sh
cmake -S . -B build/target-release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF -DCMAKE_INSTALL_LIBDIR=lib
cmake --build build/target-release
cmake --install build/target-release --prefix "$PWD/build/target-release/stage" --strip
sh scripts/measure-size.sh build/target-release/stage
```

以上默认使用本机编译器，不代表目标设备交叉编译。
交叉编译时，在首次配置新的构建目录时传入 `-DCMAKE_TOOLCHAIN_FILE=...`，
指向与设备 SDK 匹配的配置，参见 `cmake/toolchains/README.md`。
交叉编译的测试程序需在目标设备或适当模拟环境运行；产物剥离需使用目标工具链。

安装包括 `bin/vasletd`、`bin/vasletctl`、`lib/libvaslet.so`（macOS 为 `.dylib`）、
公共头文件、API 文档和许可证。CLI 通过相对安装路径查找配套动态库。
自定义安装目录时应相应调整运行时库搜索路径。

## GitHub Actions

[构建工作流](../.github/workflows/build.yml) 在 push、pull request 和手动触发时运行。
使用 Ubuntu 24.04，分别以 GCC 13 和 Clang 18 构建 Debug、Release，共四组任务。
每组运行 CTest、客户端示例，安装三个交付产物，并验证安装后的程序和动态库加载。

Release 产物剥离符号后运行体积统计，结果显示在任务摘要中。
每种编译器分别上传安装目录的 `.tar.gz` 包和体积、工具链报告，保留 7 天；
归档中包含程序、动态库、公共头文件、API 说明和许可证。先打 tar 包以保留可执行权限。
测试失败时另行上传可用的 CTest 日志。

这是 Linux x86_64 本机构建，不代表嵌入式设备交叉编译或体积验收。
CI 目前不按 100 KB 阈值判定失败，依赖大小和验收口径仍按 [体积说明](size.md) 处理。

Action 用法参考官方 [checkout](https://github.com/actions/checkout) 和
[upload-artifact](https://github.com/actions/upload-artifact) 文档。
