#include "../../include/graphics.h"

#ifdef _WIN32
#include <windows.h>
int gotoxy(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hConsole, &mode);
    if (SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        printf("\033[%d;%dH", y, x);
    } 
    else {
        COORD coord = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(hConsole, coord);
    }

    fflush(stdout);
    retrun 1;
}
#else
int gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
    return 1;
}
#endif

#ifdef _WIN32
#include <windows.h>
int clear_screen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(hConsole, &mode) &&
        SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        printf("\033[2J\033[H");
    } 
    else {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD count;
        DWORD cellCount;
        COORD homeCoords = { 0, 0 };

        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            cellCount = csbi.dwSize.X * csbi.dwSize.Y;
            FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
            FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);
            SetConsoleCursorPosition(hConsole, homeCoords);
        }
    }

    fflush(stdout);
    return 1;
}
#else
int clear_screen() {
    printf("\033[2J\033[H");
    fflush(stdout);
    return 1;
}
#endif


#ifdef _WIN32
#include <windows.h>
int get_console_size(point_t* sizes) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        sizes->x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        sizes->y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } 
    else {
        sizes->x = 80;
        sizes->y = 25;
    }

    return 1;
}

#else
#include <sys/ioctl.h>
#include <unistd.h>
int get_console_size(point_t* sizes) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
        sizes->x = ws.ws_col;
        sizes->y = ws.ws_row;
    } 
    else {
        sizes->x = 80;
        sizes->y = 25;
    }

    return 1;
}
#endif

void fprintfl(FILE* stream, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);
    fflush(stream);
}

#ifdef _WIN32
#include <conio.h>

char wait_any_key() {
    return _getch();
}

char wait_key(int target) {
    char ch = 0;
    do {
        ch = _getch();
    } 
    while (ch != target);
    return ch;
}

#else
#include <termios.h>
#include <unistd.h>
char getch_unix() {
    struct termios oldt, newt;
    char ch = 0;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

char wait_any_key() {
    return getch_unix();
}

char wait_key(char target) {
    char ch = 0;
    do {
        ch = getch_unix();
    } 
    while (ch != target);
    return ch;
}
#endif

int print_center(const char* text, int y) {
    point_t screen_sizes;
    get_console_size(&screen_sizes);
    gotoxy((screen_sizes.x / 2) - (strlen(text) / 2), MAX(MIN((screen_sizes.y / 2) + y, screen_sizes.y), 0));
    fprintfl(stdout, "%s", text);
    return 1;
}
