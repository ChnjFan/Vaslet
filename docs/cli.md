# 命令行接口

`vasletd` 中的 `d` 表示 daemon，是后台配置服务。`vasletctl` 是用于查询、修改和同步配置的命令行工具；这些配置操作尚未实现。

当前两者只提供骨架入口：

| 参数 | 行为 | 退出码 |
| --- | --- | --- |
| `--help`、`-h` | 输出使用说明及已注册参数列表 | 0 |
| `--version`、`-v` | 输出版本 | 0 |
| 无参数或仅有 `--` | `vasletd` 无输出退出；`vasletctl` 向标准错误报告功能尚未实现 | 1 |
| 未知选项、给开关赋值或位置参数 | 向标准错误报告无效参数并输出使用说明 | 1 |

`vasletd` 另支持 `--config path`、`-c path`、`--config=path`、`-cpath`，
将路径传给服务端入口；未指定时路径为 `/etc/vasletd_conf.json`。
默认路径由服务端内部头文件 [defaults.h](../src/vasletd/defaults.h) 中的
`VASLETD_DEFAULT_CONFIG_PATH` 宏定义。
目前只解析路径，不读取文件或启动服务，单独指定路径时无输出并返回 1。
`vasletctl` 尚未注册此选项。

支持重复开关、混用长短选项和短选项组合（如 `-hv`）。所有参数校验通过后，
帮助优先于版本输出；例如 `--help --version` 输出帮助，`--help --unknown` 则失败。
`--` 表示选项结束；当前不支持位置参数，因此它后面不能再有参数。

例如：

```sh
./build/host-debug/bin/vasletd -h
./build/host-debug/bin/vasletctl --version
```

两个程序使用 `src/common/command_line.h` 中的内部解析器，采用
[cmdline](https://github.com/tanakh/cmdline/blob/master/README.rst) 的注册后解析方式。
调用 `AddFlag` 注册长选项名、可选的单字符短选项和帮助文本，随后调用 `Parse`
校验参数、`Has` 查询开关，`PrintUsage` 按注册顺序生成使用说明。
调用 `AddOption` 可注册带字符串值的选项，`Get` 获取解析到的值，例如：

```cpp
vaslet::common::CommandLine arguments("example");
arguments.AddOption("config", 'c', "Path to the configuration file");
if (!arguments.Parse(argc, argv)) {
    arguments.PrintUsage(stderr);
    return 1;
}
if (arguments.Has("config")) {
    const std::string_view path = arguments.Get("config");
    // Use path before the argv strings are destroyed or changed.
}
```

带值选项必须提供一个值，支持空字符串；未指定、未注册的选项或无值开关，`Get` 返回空视图，
可通过 `Has` 区分未指定与显式空值。重复选项以最后一次的值为准，每次 `Parse` 会重置此前的状态和值。
组合短选项中，带值选项会把该组选项的剩余字符作为值，例如 `-hcpath` 表示 `-h -c path`。
分开传值时，下一个参数以 `-` 开头会被视为缺值；此类值应使用 `--config=-file` 或 `-c-file`。
值不复制，`Get` 返回引用 `argv` 字符串的 `std::string_view`，需要长期保存时应复制为 `std::string`。

当前未引入 cmdline 依赖，也不提供其类型转换、必填参数或值约束功能。

入口中的注册方式如下（字符串需在解析器存活期间保持有效，选项名不得重复）：

```cpp
vaslet::common::CommandLine arguments("vasletctl");
arguments.AddFlag("help", 'h', "Show this help message");
arguments.AddFlag("version", 'v', "Show the program version");
```

`vasletctl --version` 通过客户端动态库的 `vaslet::Version()` 获取版本，用于验证链接。
`vasletd` 尚不会启动配置服务。配置文件读取、配置操作及其他服务运行参数待实现。
