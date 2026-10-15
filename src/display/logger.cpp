#include "haws/logger/logger.h"
#include "haws/config.hpp"
#include "lemlib/Timer.hpp"
#include <iomanip>
#include <ios>
#include <sstream>


using namespace pros;

namespace logger {

std::vector<std::string> logList;
Mutex logMutex; // 互斥锁
int current = 0;
lemlib::Timer timer(0);

void init() {
    // nothing to do
    // TODO: 文件存储初始化
    timer.set(0);
}

void display() {
    logMutex.take(200);
    int size = logList.size();
    if (size < 6) {
        for (int i = 0; i < size; i++) {
            pros::lcd::print(i, logList.at(i).c_str());
        }
    } else {
        for (int i = 0; i < 6; i++) {
            pros::lcd::print(i, logList.at(i + current).c_str());
        }
    }
    logMutex.give();
}

void pageUp() {
    current = CONSTRAIN(current - 1, 0, current);
    pros::delay(20);
    display();
}

void pageDown() {
    current = CONSTRAIN(current + 1, current, logList.size() - 6);
    pros::delay(20);
    display();
}

// void log(std::string content) {
//     // if (logList.size() == 6) {
//     //     logList.erase(logList.begin());
//     // }
//     logMutex.take(500);
//     logList.push_back(content);
//     pros::delay(20);
//     int size = logList.size();
//     logMutex.give();
//     current = size - 6;
//     display();
// }

void info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    std::string content = "";

    // 按格式解析日志内容
    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'b': {
                    bool value = va_arg(args, int);
                    content += (value ? "true" : "false");
                    break;
                }
                case 'd': {
                    int value = va_arg(args, int);
                    content += std::to_string(value);
                    break;
                }
                case 'f': {
                    float value = va_arg(args, double);
                    std::ostringstream stream;
                    stream << std::fixed << std::setprecision(2) << value;
                    content += stream.str();
                    break;
                }
                case 's': {
                    char* value = va_arg(args, char *);
                    content += value;
                    break;
                }
                default: {
                    content += '%';
                }
            }
        } else {
            content += *format;
        }
        format++;
    }
    va_end(args);
    float time = timer.getTimePassed() / 1000.0;
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(1) << time;
    content = "[" + stream.str() + "]" + content;
    
    // 添加至日志列表中
    logMutex.take(500);
    logList.push_back(content);
    pros::delay(20);
    int size = logList.size();
    logMutex.give();
    current = CONSTRAIN(size - 6, 0, size - 1);
    display();
}

}
