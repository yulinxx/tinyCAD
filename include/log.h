// log
// #define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>

#define WRITE_INFOLOG(s) (LOG(INFO)<<(s)) //输出一个Info日志
#define WRITE_WARNLOG(s) (LOG(WARNING)<<(s)) //输出一个Warning日志
#define WRITE_ERRORLOG(s) (LOG(ERROR)<<(s)) //输出一个Error日志
#define WRITE_FATALLOG(s) (LOG(FATAL)<<(s)) //输出一个Fatal日志，这是最严重的日志并且输出之后会中止程序


// LOG_IF(INFO, num > 10) << "Got lots of cookies";  //当条件满足时输出日志