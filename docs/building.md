# 构建与安装

需要 CMake 3.20 或更新版本，以及支持 C++17 的编译器。当前无第三方依赖。
正式部署目标为 Linux；macOS 可用于本机骨架验证，动态库后缀为 `.dylib`。

## 本机开发

在仓库根目录执行：

```sh
cmake -S . -B build/host-debug -DCMAKE_BUILD_TYPE=Debug -DVASLET_BUILD_EXAMPLES=ON
cmake --build build/host-debug
ctest --test-dir build/host-debug --output-on-failure
./build/host-debug/examples/client/vaslet_client_example
```

交付产物的目标和安装规则集中在 `src/CMakeLists.txt`，根目录构建文件负责项目设置和子目录组织。
未安装的服务端和 CLI 分别位于构建目录的 `src/vasletd` 和 `src/vasletctl`。

测试覆盖 C++ 调用方的编译链接、帮助与版本参数，以及未实现操作的错误退出。
尚无配置读写测试。版本接口示例不需要启动服务端。

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
