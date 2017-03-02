#pragma once

/*
 * 此文件用于定义std::cout文字颜色
 * 支持 Windows 和 Linux
 */

#ifdef WIN32
    #include <wtypes.h>
    inline std::ostream& WHITE(std::ostream &s) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);  return s; }
    inline std::ostream& RED(std::ostream &s)   { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);  return s; }
    inline std::ostream& GREEN(std::ostream &s) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN |FOREGROUND_INTENSITY);  return s; }
#else
    #define WHITE   "\033[1;37m"
    #define RED     "\033[0;31m"
    #define GREEN   "\033[0;32m"
    #define BLACK   "\033[0;30m"
    #define BLUE    "\033[0;34m"
    #define YELLOW  "\033[1;33m"
#endif

