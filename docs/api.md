# 客户端 API

当前仅提供用于验证动态库交付的版本接口，尚未实现配置访问。
项目统一使用 C++17，公共接口位于 `vaslet` 命名空间；配置 API 的签名及兼容性约定将在接口设计阶段确定。

公共头文件为 `include/vaslet/vaslet.h`，链接库为 `libvaslet`。

```cpp
namespace vaslet {
const char* Version() noexcept;
}
```

返回客户端库版本字符串。指针指向库内静态存储，调用方不得修改或释放。
该接口不连接服务端。最小示例见 `examples/client/main.cpp`。

连接、读写、错误码、线程安全和其他资源生命周期约定待补充。
