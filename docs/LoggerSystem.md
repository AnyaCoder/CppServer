## ��־ϵͳ����漰��ģʽ������������ 

go back [here](../README.md)

### Tree Structure

```plaintext
LoggerManager
  ������ Logger
  ��     ������ name: string
  ��     ������ level: LogLevel
  ��     ������ formatter: LogFormatter
  ��     ������ appenders: List<LogAppender>
  ��           ������ StdoutLogAppender
  ��           ��     ������ formatter: LogFormatter
  ��           ��     ������ level: LogLevel
  ��           ������ FileLogAppender
  ��                 ������ filename: string
  ��                 ������ filestream: ofstream
  ��                 ������ level: LogLevel
  ��
  ������ LogFormatter
  ��     ������ pattern: string
  ��     ������ items: List<FormatItem>
  ��           ������ MessageFormatItem
  ��           ������ LevelFormatItem
  ��           ������ DateTimeFormatItem
  ��           ������ ...
  ��
  ������ LogEvent
  ��     ������ logger: Logger
  ��     ������ level: LogLevel
  ��     ������ file: string
  ��     ������ line: int
  ��     ������ elapse: uint32
  ��     ������ thread_id: uint32
  ��     ������ fiber_id: uint32
  ��     ������ ss: stringstream
  ��
  ������ LogEventWrap
        ������ event: LogEvent
        ������ getSS(): stringstream


```

#### һ���漰�����ģʽ 

1. **����ģʽ��Singleton Pattern��**  
  - **`LoggerManager`**  ���õ���ģʽ����֤����ϵͳ��ֻ��һ����־������ʵ�����������������־����

  - ʹ�� `anya::Singleton<LoggerManager>` ��ȷ����־������ʵ��������Ӧ����Ψһ��

2. **����ģʽ��Strategy Pattern��**  
  - **`LogAppender`**  �����˳�����ӿڣ���ͨ����ͬ��ʵ���ࣨ`StdoutLogAppender` �� `FileLogAppender`��ʵ�ֶ�����־������ԡ�

  - ��־�����Ը����������ò�ͬ�� Appender ʵ�֣�ѡ�����������̨���ļ���

3. **װ����ģʽ��Decorator Pattern��**  
  - **`LogEventWrap`**  ���ڷ�װ��־�¼���ȷ���ڶ�������ʱ�Զ�����־�¼�д���Ӧ����־���У����ٿ����߹�����־����ĸ����ԡ�

4. **�۲���ģʽ��Observer Pattern��**  
  - **`Logger`**  ��Ϊ���۲��ߣ�**`Logger`**  ��Ϊ���۲��ߣ�**`LogAppender`**  ��Ϊ�۲��ߡ�����־�¼�����ʱ��`Logger` ��֪ͨ���и��ӵ� Appender ִ����Ӧ����־�����

#### ������Ҫ���༰�䷽�� 

1. **LoggerManager**  
  - ����������־�� (`Logger`) ʵ������ά��һ��Ĭ�ϵĸ���־����

  - ͨ�� `getLogger()` ��ȡָ�����Ƶ���־��������������򷵻ظ���־����

2. **Logger**  
  - ���������־��¼�߼���ά����� `Appender`��֧�ֶ��������ʽ��

  - ��־�ȼ��Ŀ��ƣ�`Logger` ͨ����־�ȼ����˲���Ҫ�������
�����ȼ���DEBUG��INFO��WARN��ERROR��FATAL��

3. **LogAppender**  
  - ������־����ӿڣ�����ʵ�ְ��� `StdoutLogAppender`�����������̨�� �� `FileLogAppender`��������ļ�����

4. **LogFormatter**  
  - �����ʽ����־���ݣ�ͨ��ռλ������ `%d`��`%t`��`%m`����̬������־��ʽ��

5. **LogEvent** 
  - ��ʾһ�ξ������־�¼�������������־����������Ϣ���ļ������кš��߳� ID �ȣ���

6. **LogEventWrap**  
  - ��װ `LogEvent`��ȷ���¼�������ʱ�Զ�д����־����


---


#### ������־��ʼ������ 

1. **���� Logger ʵ����**  
  - ����һ����־��ʵ����Ĭ����־����Ϊ `DEBUG`�������������ƻ���Ĭ�ϵ� "root"��


```cpp
anya::Logger::ptr logger(new anya::Logger("root"));
```

2. **��� Appender����־����أ���** 
  - ��������̨���ļ������ Appender������������ӵ���־����


```cpp
logger->addAppender(anya::LogAppender::ptr(new anya::StdoutLogAppender));
anya::FileLogAppender::ptr file_appender(new anya::FileLogAppender("./log.txt"));
logger->addAppender(file_appender);
```

3. **������־��ʽ��** 
  - ������־��ʽ����Ϊ Appender ָ����ʽ���磺ʱ�䡢��־������Ϣ���ݣ���


```cpp
anya::LogFormatter::ptr fmt(new anya::LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%p%T%m%n"));
file_appender->setFormatter(fmt);
```

4. **������־�������**  
  - Ϊ�ļ���� Appender ������־����Ϊ `ERROR`��ʹ��ֻ����������ϼ������־��


```cpp
file_appender->setLevel(anya::LogLevel::ERROR);
```


---


#### �ġ���־������� 

1. **������־�¼���** 
  - ʹ�ú궨��Ŀ�ݷ�ʽ�����־��


```cpp
ANYA_LOG_INFO(logger) << "This is an info message.";
ANYA_LOG_ERROR(logger) << "This is an error message.";
```

2. **��־���ˣ�**  
  - ��־�����ݵ�ǰ��־������˲�������������־�¼���
���磬�����ǰ����Ϊ `INFO`���򲻻���� `DEBUG` �������־��

3. **�¼���װ����ʽ����**  
  - ��־�¼� (`LogEvent`) ����װ�� `LogEventWrap` �У�ȷ�������������ڽ���ʱ�Զ������

  - `LogFormatter` ���ݶ���ĸ�ʽ���ַ������¼���ʽ��Ϊ�ַ�����

4. **����� Appender��**  
  - ��־���������и��ӵ� Appender�����������ǵ� `log()` ����������ʽ�������־���������̨���ļ���


---


#### �塢���������������� 

1. **`main()` �г�ʼ����־����** 
  - ��ʼ����־���Ͷ�Ӧ�� Appender��

2. **������־�¼���**  
  - ͨ���궨����� `ANYA_LOG_*` ��ݷ�ʽ������־�¼���

3. **��־�¼���װ��������**  
  - ʹ�� `LogEventWrap` �Զ������¼�������������ֶ�������־�����á�

4. **����̨���ļ������** 
  - ���ϵȼ�����־�¼��ᱻ��ʽ�������������̨��ָ���ļ���
