## 日志系统设计涉及的模式、方法及流程 

go back [here](../README.md)

### Tree Structure

```plaintext
LoggerManager
  ├── Logger
  │     ├── name: string
  │     ├── level: LogLevel
  │     ├── formatter: LogFormatter
  │     └── appenders: List<LogAppender>
  │           ├── StdoutLogAppender
  │           │     ├── formatter: LogFormatter
  │           │     └── level: LogLevel
  │           └── FileLogAppender
  │                 ├── filename: string
  │                 ├── filestream: ofstream
  │                 └── level: LogLevel
  │
  ├── LogFormatter
  │     ├── pattern: string
  │     └── items: List<FormatItem>
  │           ├── MessageFormatItem
  │           ├── LevelFormatItem
  │           ├── DateTimeFormatItem
  │           └── ...
  │
  ├── LogEvent
  │     ├── logger: Logger
  │     ├── level: LogLevel
  │     ├── file: string
  │     ├── line: int
  │     ├── elapse: uint32
  │     ├── thread_id: uint32
  │     ├── fiber_id: uint32
  │     └── ss: stringstream
  │
  └── LogEventWrap
        ├── event: LogEvent
        └── getSS(): stringstream


```

#### 一、涉及的设计模式 

1. **单例模式（Singleton Pattern）**  
  - **`LoggerManager`**  采用单例模式，保证整个系统中只有一个日志管理器实例，负责管理所有日志器。

  - 使用 `anya::Singleton<LoggerManager>` 来确保日志管理器实例在整个应用中唯一。

2. **策略模式（Strategy Pattern）**  
  - **`LogAppender`**  定义了抽象类接口，并通过不同的实现类（`StdoutLogAppender` 和 `FileLogAppender`）实现多种日志输出策略。

  - 日志器可以根据需求设置不同的 Appender 实现，选择输出到控制台或文件。

3. **装饰器模式（Decorator Pattern）**  
  - **`LogEventWrap`**  用于封装日志事件，确保在对象析构时自动将日志事件写入对应的日志器中，减少开发者管理日志输出的复杂性。

4. **观察者模式（Observer Pattern）**  
  - **`Logger`**  作为被观察者，**`Logger`**  作为被观察者，**`LogAppender`**  作为观察者。当日志事件触发时，`Logger` 会通知所有附加的 Appender 执行相应的日志输出。

#### 二、主要的类及其方法 

1. **LoggerManager**  
  - 管理所有日志器 (`Logger`) 实例，并维护一个默认的根日志器。

  - 通过 `getLogger()` 获取指定名称的日志器，如果不存在则返回根日志器。

2. **Logger**  
  - 负责核心日志记录逻辑，维护多个 `Appender`，支持多种输出方式。

  - 日志等级的控制：`Logger` 通过日志等级过滤不必要的输出。
常见等级：DEBUG、INFO、WARN、ERROR、FATAL。

3. **LogAppender**  
  - 定义日志输出接口，具体实现包括 `StdoutLogAppender`（输出到控制台） 和 `FileLogAppender`（输出到文件）。

4. **LogFormatter**  
  - 负责格式化日志内容，通过占位符（如 `%d`、`%t`、`%m`）动态生成日志格式。

5. **LogEvent** 
  - 表示一次具体的日志事件，包含发生日志的上下文信息（文件名、行号、线程 ID 等）。

6. **LogEventWrap**  
  - 包装 `LogEvent`，确保事件在析构时自动写入日志器。


---


#### 三、日志初始化流程 

1. **创建 Logger 实例：**  
  - 创建一个日志器实例，默认日志级别为 `DEBUG`，可以设置名称或保留默认的 "root"。


```cpp
anya::Logger::ptr logger(new anya::Logger("root"));
```

2. **添加 Appender（日志输出地）：** 
  - 创建控制台和文件输出的 Appender，并将它们添加到日志器。


```cpp
logger->addAppender(anya::LogAppender::ptr(new anya::StdoutLogAppender));
anya::FileLogAppender::ptr file_appender(new anya::FileLogAppender("./log.txt"));
logger->addAppender(file_appender);
```

3. **设置日志格式：** 
  - 配置日志格式器，为 Appender 指定格式（如：时间、日志级别、消息内容）。


```cpp
anya::LogFormatter::ptr fmt(new anya::LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%p%T%m%n"));
file_appender->setFormatter(fmt);
```

4. **设置日志输出级别：**  
  - 为文件输出 Appender 设置日志级别为 `ERROR`，使其只输出错误及以上级别的日志。


```cpp
file_appender->setLevel(anya::LogLevel::ERROR);
```


---


#### 四、日志输出流程 

1. **触发日志事件：** 
  - 使用宏定义的快捷方式输出日志：


```cpp
ANYA_LOG_INFO(logger) << "This is an info message.";
ANYA_LOG_ERROR(logger) << "This is an error message.";
```

2. **日志过滤：**  
  - 日志器根据当前日志级别过滤不符合条件的日志事件。
例如，如果当前级别为 `INFO`，则不会输出 `DEBUG` 级别的日志。

3. **事件封装及格式化：**  
  - 日志事件 (`LogEvent`) 被封装到 `LogEventWrap` 中，确保其在生命周期结束时自动输出。

  - `LogFormatter` 根据定义的格式化字符串将事件格式化为字符串。

4. **输出到 Appender：**  
  - 日志器遍历所有附加的 Appender，并调用它们的 `log()` 方法，将格式化后的日志输出到控制台或文件。


---


#### 五、完整代码运行流程 

1. **`main()` 中初始化日志器：** 
  - 初始化日志器和对应的 Appender。

2. **生成日志事件：**  
  - 通过宏定义调用 `ANYA_LOG_*` 快捷方式生成日志事件。

3. **日志事件封装及析构：**  
  - 使用 `LogEventWrap` 自动处理事件的输出，避免手动管理日志器调用。

4. **控制台及文件输出：** 
  - 符合等级的日志事件会被格式化后输出到控制台和指定文件。
