#include "../anya/log.h"
#include "../anya/util.h"


int main(int argc, char** argv) {
	anya::Logger::ptr logger(new anya::Logger);
	logger->addAppender(anya::LogAppender::ptr(new anya::StdoutLogAppender));
	
	anya::FileLogAppender::ptr file_appender(new anya::FileLogAppender("./log.txt"));
	logger->addAppender(file_appender);

	anya::LogFormatter::ptr fmt(new anya::LogFormatter("%d%T%p%T%m%n"));
	file_appender->setFormatter(fmt);

	file_appender->setLevel(anya::LogLevel::ERROR);

	std::cout << "hello anya log" << std::endl;
	ANYA_LOG_INFO(logger) << "test macro";
	ANYA_LOG_ERROR(logger) << "test macro error";
	ANYA_LOG_FMT_ERROR(logger, "test macro fmt error %s", "bb");

	auto l = anya::LoggerMgr::GetInstance()->getLogger("xx");
	ANYA_LOG_INFO(l) << "xxxx";
	return 0;
}