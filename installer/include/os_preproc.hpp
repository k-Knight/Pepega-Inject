#ifndef PEPEGA_OS
#define PEPEGA_OS

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
    #define OS_WIN
#endif

#endif