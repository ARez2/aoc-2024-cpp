#pragma once
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace term {

struct Color {
    int code;
};
inline Color Red{31};
inline Color Green{32};
inline Color Yellow{33};
inline Color Blue{34};
inline Color Reset{0};

// Enable ANSI escape codes on Windows (no-op on Linux/macOS)
inline void init() {
#ifdef _WIN32
    static bool initialized = false;
    if (!initialized) {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD mode = 0;
            if (GetConsoleMode(hOut, &mode)) {
                SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
        }
        initialized = true;
    }
#endif
}

inline void clear() {
    std::cout << "\033[2J\033[H";
}

inline void set_color(Color color) {
    std::cout << "\033[" << static_cast<int>(color.code) << "m";
}

inline void reset_color() {
    std::cout << "\033[" << Reset.code << "m";
}

inline std::ostream &operator<<(std::ostream &os, Color c) {
    return os << "\033[" << c.code << "m";
}

} // namespace term