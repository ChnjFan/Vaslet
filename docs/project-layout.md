# 项目目录规划

Vaslet 交付服务端、客户端动态库及公共 API、命令行控制程序，三个编译产物总体积目标小于 100 KB。

以下目录结构已生成最小骨架，使用 CMake 构建，全部源文件采用 C++17，公共接口位于 `vaslet` 命名空间；配置 API、通信方式和第三方依赖另行确定。

## 目录结构

```text
Vaslet/
├── CMakeLists.txt             # 项目设置、构建选项与子目录组织
├── .clang-format             # Google C++ 格式化配置
├── README.md                 # 英文项目入口
├── LICENSE
├── include/
│   └── vaslet/               # 随客户端库交付的公共头文件
│       └── vaslet.h           # 客户端公共 API 声明
├── src/
│   ├── CMakeLists.txt        # 三个交付产物的目标、依赖和安装规则
│   ├── common/               # 内部共享代码：协议、错误处理等
│   ├── server/               # 服务端：请求处理、配置管理与持久化
│   │   └── main.cpp          # 服务端入口
│   ├── client/               # 客户端库：公共 API 实现、连接与请求封装
│   └── cli/                  # 命令行参数处理、结果输出
│       └── main.cpp          # 命令行入口
├── cmake/
│   └── toolchains/           # 目标设备的交叉编译工具链配置
├── tests/
│   ├── unit/                 # 配置处理、协议与参数解析等单元测试
│   ├── integration/          # 服务端、客户端库与 CLI 联调测试
│   └── fixtures/             # 测试用 JSON 与异常输入
├── examples/
│   ├── client/               # 公共 API 的最小调用示例
│   └── config.json           # 示例配置
├── scripts/
│   └── measure-size.sh       # 统计安装后产物体积与直接依赖
├── docs/
│   ├── README_zh.md          # 中文项目入口
│   ├── project-layout.md     # 本文档
│   ├── api.md                # API 约定、错误处理与资源生命周期
│   ├── cli.md                # 命令、参数、退出码与脚本用法
│   ├── protocol.md           # 内部通信协议
│   ├── building.md           # 本机构建、交叉编译与安装
│   └── size.md               # 体积统计口径与测量结果
└── build/                    # 本地构建与安装暂存目录，不纳入版本控制
```

## 交付与构建目标

根目录 `CMakeLists.txt` 负责项目设置与子目录组织，`src/CMakeLists.txt` 定义三个交付产物及其依赖、包含路径和安装规则。以下构建目标和安装规则已实现；服务端与 CLI 目前仅支持帮助和版本查询。

| 交付内容 | 源码位置 | 建议构建目标 | 安装位置 |
| --- | --- | --- | --- |
| 配置服务端 | `src/server/` | `vasletd` | `bin/vasletd` |
| 客户端动态库 | `src/client/` | `vaslet_client`，生成 `libvaslet.so` | `lib/libvaslet.so` |
| 公共 API 头文件 | `include/vaslet/` | 随客户端库安装 | `include/vaslet/` |
| 命令行控制程序 | `src/cli/` | `vasletctl` | `bin/vasletctl` |

API 声明与实现分别放在 `include/vaslet/` 和 `src/client/`。API 文档随开发包交付；头文件、文档和示例不属于编译产物体积统计对象。

## 组件边界

- `include/vaslet/` 只包含对外接口，不暴露服务端内部类型、通信协议或第三方库头文件。公共接口统一使用 C++17，配置访问接口另行设计。
- `src/server/` 负责配置的服务端处理。业务程序不直接依赖这里的实现。
- `src/client/` 实现公共接口，封装与服务端的交互。
- `src/cli/` 通过客户端动态库访问配置，仅保留命令行解析和输出逻辑。
- `src/common/` 按需为服务端和客户端提供内部共享代码，不单独交付动态库。内部头文件与对应源码放在一起。
- JSON 解析等依赖由实际使用它们的组件引入，避免无关组件承担额外代码体积。
- 测试、示例和开发工具不链接进正式交付产物。

## 构建与体积管理

采用源码外构建，不将生成文件写入 `src/` 或 `include/`。建议按配置分开存放：

```text
build/
├── host-debug/               # 本机开发与测试
└── target-release/           # 目标设备发布构建
    └── stage/               # 按安装规则暂存的交付文件
```

`cmake/toolchains/` 只记录工具链配置，不存放编译器、SDK 或设备固件。第三方依赖确定后再决定集成方式，当前不预建 `third_party/` 目录。

体积统计应针对同一次目标设备 Release 构建的安装产物，合计 `vasletd`、`libvaslet.so` 和 `vasletctl` 的实际文件大小；动态库符号链接不重复计数。统计脚本应同时列出运行时依赖，避免遗漏新增的部署成本。

`docs/size.md` 记录目标架构、编译器版本、编译和链接选项、符号剥离方式、各产物字节数及合计值。100 KB 的字节口径、额外依赖是否计入预算需在验收前确定；运行时内存占用单独报告。

## 落地顺序

1. 已添加根构建入口、公共头文件与三个组件的最小实现，形成可构建、可安装的交付骨架。
2. 实现一次完整的配置访问流程，并添加对应的集成测试和 API 示例。
3. 在目标工具链上构建发布产物，建立体积统计基线，再根据测量结果优化。
4. 随功能实现补充 API、CLI、协议和构建文档。
