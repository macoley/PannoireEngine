
#include "PE/Utils/ConsoleLogger.h"

void PE::Utils::ConsoleLogger::log(const std::string & text) const {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::cout << "<" << std::put_time(&tm, "%d/%m/%y %H:%M:%S") << "> " << text << std::endl;
}
