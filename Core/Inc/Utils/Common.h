#ifndef RENSSELAERMOTORSPORT_COMMON_H
#define RENSSELAERMOTORSPORT_COMMON_H

// Definition of common constants and macros used throughout the project
#define _SUCCESS 0
#define _FAILURE 1

// Print colors for terminal output
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define ESC "\x1b["
#define ESC_AQUA ESC "36m"
#define ESC_GREEN ESC "32m"
#define ESC_RED ESC "31m"
#define ESC_RESET ESC "0m"

#define TEST_DELIM "\t"
#define TEST_START ESC_AQUA "START" ESC_RESET TEST_DELIM
#define TEST_PASS ESC_GREEN "PASS" ESC_RESET TEST_DELIM
#define TEST_OK ESC_GREEN "OK" ESC_RESET TEST_DELIM
#define TEST_ERR ESC_RED "ERR" ESC_RESET TEST_DELIM
#define TEST_FAIL ESC_RED "FAIL" ESC_RESET TEST_DELIM


#endif // RENSSELAERMOTORSPORT_COMMON_H
