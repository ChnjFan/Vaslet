Vaslet 是面向低成本 Linux 嵌入式设备的轻量级 JSON 配置中心，提供服务端、客户端动态库及公共 API，以及命令行控制程序，目标是将全部交付编译产物的总体积控制在 100 KB 以下。

> **Vaslet 处于初始开发阶段。** 以下功能描述的是计划实现的范围，目前尚未提供可用版本或体积测量结果。

[English](../README.md) | 简体中文

# 组件

| 组件 | 名称 | 说明 |
| --- | --- | --- |
| 服务端 | `vasletd` | `d` 表示 daemon，后台配置服务 |
| 客户端动态库 | `libvaslet` | Linux 下产物为 `libvaslet.so` |
| 命令行 | `vasletctl` | 用于查询、修改和同步配置（待实现） |

# 功能

计划实现的功能：

* 使用 JSON 表示配置数据。
* 通过独立服务端集中管理设备上各应用的配置。
* 业务程序通过客户端动态库和公共 API 访问配置。
* 提供命令行控制程序，用于设备调试与脚本调用。
* 服务端、客户端动态库和命令行程序的编译产物合计小于 100 KB。
* 控制依赖与运行时资源开销，适应嵌入式设备的资源限制。

# 文档

[英文 README](../README.md) 随源码提供。另见 [API 说明](api.md)、[命令行接口](cli.md)和[构建与安装](building.md)。

# 仓库结构

计划将公共 API 头文件放在 `include/vaslet/`，服务端、客户端库和命令行实现分别放在 `src/server/`、`src/client/` 和 `src/cli/`，内部共享代码放在 `src/common/`。测试、示例和构建工具独立存放。

完整目录树、组件边界和交付目标见[项目目录规划](project-layout.md)。当前已生成可构建的最小骨架。

# 限制

* 目标平台为嵌入式 Linux，尚未定义其他操作系统的支持范围。
* 当前仅有可构建骨架，支持帮助和版本查询，尚未实现配置访问与通信。配置 API 和命令语法尚未确定。
* 100 KB 体积目标尚未验证，运行时内存占用需单独测量。

# 获取源码

```sh
git clone https://github.com/ChnjFan/Vaslet.git
```

# 构建

骨架使用 CMake 3.20 或更新版本、C++17，无第三方依赖。

```sh
cmake -S . -B build/host-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/host-debug
ctest --test-dir build/host-debug --output-on-failure
```

发布安装、示例程序及交叉编译说明见[构建与安装](building.md)。当前尚不支持配置操作。


GitHub Actions 使用 GCC、Clang 在 Linux 上构建和测试 Debug、Release，
并上传 Release 安装包和体积报告。详情见 [CI 说明](building.md#github-actions)。

# 参与开发

C++ 代码遵循 Google C++ 风格，格式化规则由 [`.clang-format`](../.clang-format) 定义。在仓库根目录执行以下命令可格式化指定源文件：

```sh
clang-format -i --style=file path/to/file.cpp
```

# 编译产物体积

体积预算包含服务端可执行文件、客户端动态库和命令行可执行文件的总和。

后续报告将注明目标架构、编译器、优化选项、是否剥离符号，以及额外运行时依赖。KB 的字节口径和依赖计入方式将在建立构建与测量流程时明确。目前尚无体积或性能测量结果。

# 许可证

本项目采用 [MIT License](../LICENSE)。
