#include "api.h"

namespace logger {
/**
 * @brief 初始化日志记录模块
 */
void init();

/**
 * @brief 打印日志
 * @param format 需要打印内容的格式 (int:%d, float:%f, bool:%b, string:%s)
 * @param ... 额外需要传入的参数列表
 */
void info(const char* format, ...);

/**
 * @brief 向上翻页
 */
void pageUp();

/**
 * @brief 向下翻页
 */
void pageDown();
}