# CppServer

## Setup
### Build from Source
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
### Run
```bash
cd bin
./CppServer
```

## Modules Introduction
### 1. Logger System

See details [here](docs/LoggerSystem.md)

1. 通过**单例模式**实现日志管理器的全局唯一实例，用策略模式支持多种日志输出（控制台和文件），并采用装饰器模式封装日志事件，确保生命周期结束时自动输出。
2. 系统使用**智能指针**管理对象生命周期，避免内存泄漏，同时通过宏定义和可变参数支持简洁灵活的日志接口。
3. 在并发环境下，它记录**线程和协程ID**，增强了日志的调试能力。
4. 此外，系统支持动态配置日志格式和输出级别，满足**企业级应用的高扩展性和可维护性**要求，是C++面试中考察代码能力与设计思想的良好示例。