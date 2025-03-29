#ifndef RENSSELAERMOTORSPORT_COMMON_H
#define RENSSELAERMOTORSPORT_COMMON_H

// Definition of common constants and macros used throughout the project
#define SUCCESS 0
#define FAILURE 1

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
