# Vaslet 项目约定

本文件适用于整个仓库。修改前先阅读相关源码、构建规则和文档，保留用户已有的未提交改动。

## 项目定位与交付

Vaslet 是面向低成本嵌入式 Linux 设备的 JSON 配置中心。

| 组件 | 名称 | 职责 |
| --- | --- | --- |
| 服务端 | `vasletd` | 后台配置服务，负责配置管理与请求处理 |
| 客户端动态库 | `libvaslet` | 提供公共 C++ API，Linux 产物为 `libvaslet.so` |
| 命令行工具 | `vasletctl` | 通过客户端库查询、修改和同步配置 |

- 三个交付编译产物的总体积目标小于 100 KB，不能按单个组件分别验收。
- 当前处于骨架开发阶段。帮助和版本查询已经实现，配置访问与通信尚未实现；文档不得将计划功能描述为可用功能。后续随实现更新状态。
- Linux 是部署目标；macOS 可用于本机开发验证，其 `.dylib` 产物和测量结果不能替代 Linux 目标设备验收。

## 语言与代码风格

- 项目统一使用 **C++17**，实现和测试源文件使用 `.cpp`，头文件使用 `.h`，不新增 `.c` 源文件或 C 构建目标。
- CMake 仅启用 `CXX`，要求 C++17，关闭编译器语言扩展。对外接口使用 `vaslet` 命名空间，不自行引入 C ABI。
- 遵循 Google C++ 风格，以根目录 `.clang-format` 为格式化依据；只格式化本次涉及的代码。
- 优先使用 RAII 明确资源所有权和生命周期，检查系统调用及配置解析的失败路径。
- 关注堆分配、数据复制和运行时内存成本，但不要为了未验证的优化牺牲接口清晰度或错误处理。
- 不在无关改动中全局关闭异常、RTTI 或编译警告；需要调整编译策略时说明原因和验证结果。

格式化指定文件：

```sh
clang-format -i --style=file path/to/file.cpp
```

## 目录与依赖边界

- 根目录 `CMakeLists.txt`：项目设置、C++ 标准、构建选项和子目录组织。
- `src/CMakeLists.txt`：三个交付目标的源文件、依赖、包含路径、可见性与安装规则。
- `include/vaslet/`：对外交付的公共头文件；不暴露服务端内部类型、协议结构或第三方实现细节。
- `src/server/`：服务端实现。
- `src/client/`：公共客户端 API 实现及服务端交互封装；当前 CMake 目标为 `vaslet_client`，输出库名为 `vaslet`。
- `src/cli/`：命令行解析与输出；通过客户端库访问配置，不复制服务端或客户端的业务逻辑。
- `src/common/`：按需共享的内部实现，不单独交付动态库。
- `tests/`、`examples/`、`scripts/`：测试、调用示例和开发工具，不链接进正式交付产物。
- `cmake/toolchains/`：目标 SDK 的交叉编译配置，不提交 SDK 二进制或机器专属绝对路径。
- `docs/`：接口、命令、协议、构建、目录规划与测量说明。

不依赖源码通配自动收集文件，新增编译单元应显式加入对应目标。构建和生成文件放在 `build/`，不手工编辑或提交 `compile_commands.json`、生成头文件及编译产物。编辑器包含路径应来自实际 CMake 配置，不使用其他开发者机器上的路径。

## 构建与验证

要求 CMake 3.20 或更新版本以及支持 C++17 的编译器。从仓库根目录执行：

```sh
cmake -S . -B build/host-debug -DCMAKE_BUILD_TYPE=Debug -DVASLET_BUILD_EXAMPLES=ON
cmake --build build/host-debug
ctest --test-dir build/host-debug --output-on-failure
```

- 代码或构建规则变更后运行相关构建和测试；公共 API 改动还需验证示例程序的编译与链接。
- 添加行为或修复缺陷时，补充能验证实际行为或复现缺陷的测试。避免只重复实现的测试。
- 修改目标名称、安装规则或动态库搜索路径后，验证安装后的程序能够加载配套库并运行。
- 纯文档变更检查内容准确性、相对链接和 `git diff --check`，无需运行无关测试。
- 不将交叉编译产物直接作为本机程序执行。无法完成验证时，明确说明未运行的检查及原因，不声称检查通过。

具体构建、安装和交叉编译约定见 [docs/building.md](docs/building.md)。

## 体积与依赖约束

- 当前无第三方依赖。引入依赖前说明其用途、许可证、部署要求以及对三个产物的体积影响；依赖只链接到实际需要它的目标。
- 不把仅用于测试或示例的库带入正式交付。
- 引入依赖、明显增加代码量或变更优化选项时，测量同一工具链、同一配置下的变更前后体积。
- 发布体积以目标设备 Release 构建并安装后的实际文件为依据，记录架构、工具链、选项、符号剥离方式和额外运行时依赖；运行时内存单独报告。
- `scripts/measure-size.sh` 目前报告三个产物的字节数与直接动态依赖，不累计全部依赖体积，也不作达标判定。
- KB 的字节口径及额外依赖计入方式尚待确定，不擅自将骨架或本机结果标为满足交付目标。

统计方法见 [docs/size.md](docs/size.md)。

## 文档维护

- 根目录 `README.md` 使用英文，中文版为 `docs/README_zh.md`；项目定位、组件名称、功能状态与使用入口保持一致。
- API、CLI、协议、构建或目录变动时，同步更新对应文档和受影响的示例。
- 用可运行的命令和真实路径说明已实现功能；未实现的接口或行为明确标注为计划，不提供看似可用的虚构命令。

## Git commit message 规范

采用 Conventional Commits 格式：

```text
<type>(<scope>): <summary>

[可选正文：说明变更原因、关键行为和验证结果]

[可选页脚：关联 issue 或说明破坏性变更]
```

### 标题

- `type` 必填，小写；`scope` 可选，使用小写组件名。
- 标题使用英文，摘要以祈使动词开头，例如 `add`、`fix`、`update`、`remove`。
- 标题不超过 72 个字符，不以句号结尾；准确概括本次变更，避免 `update code`、`fix bug` 等模糊描述。
- 单次提交围绕一个明确目的，避免混入无关格式化或重构。

| type | 用途 |
| --- | --- |
| `feat` | 新增功能 |
| `fix` | 修复缺陷 |
| `docs` | 文档变更 |
| `style` | 不改变行为的格式调整 |
| `refactor` | 不新增功能、不修复缺陷的代码重构 |
| `perf` | 性能、内存或产物体积优化 |
| `test` | 测试变更 |
| `build` | 构建、依赖、工具链或打包规则变更 |
| `ci` | CI 配置变更 |
| `chore` | 其他仓库维护 |
| `revert` | 撤销已有提交，正文注明被撤销的提交 |

建议 scope：`server`、`client`、`cli`、`common`、`api`、`cmake`、`tests`、`docs`、`scripts`、`repo`。分类按变更目的选择，不按文件后缀机械分类。

### 正文与破坏性变更

- 正文与标题之间空一行，可使用中文或英文；说明为什么修改，以及调用方需要知道的影响。
- 涉及公共 API/ABI、命令行语法、协议或配置兼容性破坏时，在 type/scope 后添加 `!`，并在页脚用 `BREAKING CHANGE:` 说明影响与迁移方法。
- 只有实际执行过的检查才能写为验证通过。

以下为格式示例，不代表对应功能已实现：

```text
feat(server): add configuration request handling
fix(cli): return failure for invalid arguments
build(cmake): define delivery targets under src
perf(client): reduce request buffer allocations
docs(repo): document project and commit conventions
```

破坏性变更示例：

```text
feat(api)!: replace the configuration update interface

BREAKING CHANGE: Replace calls to the previous update interface with the new API.
```

生成提交建议不等于执行提交。用户未要求提交或推送时，不自动执行 `git commit` 或 `git push`。

<!-- CODEGRAPH_START -->
## CodeGraph

In repositories indexed by CodeGraph (a `.codegraph/` directory exists at the repo root), reach for it BEFORE grep/find or reading files when you need to understand or locate code:

- **MCP tool** (when available): `codegraph_explore` answers most code questions in one call — the relevant symbols' verbatim source plus the call paths between them, including dynamic-dispatch hops grep can't follow. Name a file or symbol in the query to read its current line-numbered source. If it's listed but deferred, load it by name via tool search.
- **Shell** (always works): `codegraph explore "<symbol names or question>"` prints the same output.

If there is no `.codegraph/` directory, skip CodeGraph entirely — indexing is the user's decision.
<!-- CODEGRAPH_END -->
