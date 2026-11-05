# 项目目录规划

Vaslet 交付服务端、客户端动态库及公共 API、命令行控制程序，三个编译产物总体积目标小于 100 KB。

以下目录结构已生成最小骨架，使用 CMake 构建，全部项目源文件采用 C++17，公共接口位于 `vaslet` 命名空间；配置 API、通信方式和运行时第三方依赖另行确定。单元测试使用 GoogleTest，由 `BUILD_TESTING` 控制。

## 目录结构

```text
Vaslet/
├── CMakeLists.txt             # 全局设置、构建开关、输出目录与安装规则
├── .clang-format             # Google C++ 格式化配置
├── README.md                 # 英文项目入口
├── LICENSE
├── include/
│   └── vaslet/               # 随客户端库交付的公共头文件
│       └── vaslet.h           # 客户端公共 API 声明
├── src/
│   ├── CMakeLists.txt        # 按目录收集源码、生成头文件并集中定义目标
│   ├── common/               # 内部共享代码：协议、错误处理等
│   │   ├── command_line.h    # 开关、字符串选项的注册、解析和帮助生成
│   │   └── command_line.cpp  # 服务端与 CLI 共用的参数解析实现
│   ├── vasletd/              # 服务端：请求处理、配置管理与持久化
│   │   ├── defaults.h        # 服务端默认配置路径宏
│   │   └── main.cpp          # 服务端入口
│   ├── libvaslet/            # 客户端库：公共 API 实现、连接与请求封装
│   └── vasletctl/            # 命令行参数处理、结果输出
│       └── main.cpp          # 命令行入口
├── cmake/
│   └── toolchains/           # 目标设备的交叉编译工具链配置
├── tests/
│   ├── CMakeLists.txt        # GoogleTest 依赖和测试目标
│   ├── unit/                 # 配置处理、协议与参数解析等单元测试
│   │   ├── api/             # 公共 API GoogleTest 测试，独立构建
│   │   └── command_line/    # 命令行解析 GoogleTest 测试，独立构建
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

根目录 `CMakeLists.txt` 参考 Jianm 的分区风格，负责全局设置、输出目录、开关和安装规则。`src/CMakeLists.txt` 按模块目录递归收集 `.cpp` 和 `.h`，集中定义各目标、依赖和包含路径；`tests/CMakeLists.txt` 集中定义测试依赖和目标。以下构建目标和安装规则已实现；服务端与 CLI 支持帮助和版本查询，服务端另可解析配置文件路径，但尚不读取文件或启动配置服务。

| 交付内容 | 源码位置 | 建议构建目标 | 安装位置 |
| --- | --- | --- | --- |
| 配置服务端 | `src/vasletd/` | `vasletd` | `bin/vasletd` |
| 客户端动态库 | `src/libvaslet/` | `vaslet_client`，生成 `libvaslet.so` | `lib/libvaslet.so` |
| 公共 API 头文件 | `include/vaslet/` | 随客户端库安装 | `include/vaslet/` |
| 命令行控制程序 | `src/vasletctl/` | `vasletctl` | `bin/vasletctl` |

API 声明与实现分别放在 `include/vaslet/` 和 `src/libvaslet/`。API 文档随开发包交付；头文件、文档和示例不属于编译产物体积统计对象。

## 组件边界

- `include/vaslet/` 只包含对外接口，不暴露服务端内部类型、通信协议或第三方库头文件。公共接口统一使用 C++17，配置访问接口另行设计。
- `src/vasletd/` 负责配置的服务端处理。业务程序不直接依赖这里的实现。
- `src/libvaslet/` 实现公共接口，封装与服务端的交互。
- `src/vasletctl/` 通过客户端动态库访问配置，仅保留命令行解析和输出逻辑。
- `src/common/` 按需为服务端和客户端提供内部共享代码，不单独交付动态库。内部头文件与对应源码放在一起。
- JSON 解析等依赖由实际使用它们的组件引入，避免无关组件承担额外代码体积。
- 测试、示例和开发工具不链接进正式交付产物。

## 构建与体积管理

采用源码外构建，不将生成文件写入 `src/` 或 `include/`。建议按配置分开存放：

```text
build/
├── host-debug/               # 本机开发与测试
│   ├── bin/                 # 程序、测试和示例
│   └── lib/                 # 客户端动态库和内部静态库
└── target-release/           # 目标设备发布构建
    ├── bin/
    ├── lib/
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
