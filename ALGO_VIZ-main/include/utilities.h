#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

// ANSI escape codes for colors
extern const std::string RESET;
extern const std::string RED;
extern const std::string GREEN;
extern const std::string YELLOW;
extern const std::string BLUE;
extern const std::string MAGENTA;
extern const std::string CYAN;
extern const std::string WHITE;

#ifdef _WIN32
void clearScreen();
#else
void clearScreen();
#endif

#endif // UTILITIES_H
