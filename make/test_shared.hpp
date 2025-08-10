#pragma once
#include <exception>

#define _STRINGIFY(X) #X
#define STRINGIFY(MACRO) _STRINGIFY(MACRO)

#define FILE_LINE_STR __FILE__":" STRINGIFY(__LINE__)

#define STATUS_MSG_STR "[\033[33mSTATUS\033[0m] "
#define SUCCESS_MSG_STR "[\033[32mSUCCESS\033[0m] "
#define ERROR_MSG_STR "[\033[31mERROR\033[0m] "
#define FAIL_MSG_STR "[\033[31mFAILED\033[0m] "

extern const char* const test_name_g;

extern void test_function();

class test_fail_exception : public std::exception
{
    const char* const message_;
public:
    inline constexpr virtual const char* what() const noexcept override
    {
        return message_;
    }
    test_fail_exception(const char* message):
        message_(message)
    {

    }
};

#define TEST_FAIL(MESSAGE) throw(test_fail_exception(MESSAGE))
#define TEST_FAIL_MSG(FORMAT, ...) std::fprintf(stdout, FAIL_MSG_STR FORMAT "\n", ##__VA_ARGS__)
#define TEST_FAIL_MSG_ERR(FORMAT, ...) std::fprintf(stderr, FAIL_MSG_STR FORMAT "\n", ##__VA_ARGS__)
#define TEST_STATUS_MSG(FORMAT, ...) std::fprintf(stdout, STATUS_MSG_STR FORMAT "\n", ##__VA_ARGS__)
#define TEST_STATUS_MSG_ERR(FORMAT, ...) std::fprintf(stderr, STATUS_MSG_STR FORMAT "\n", ##__VA_ARGS__)
#define TEST_ERROR_MSG(FORMAT, ...) std::fprintf(stdout, ERROR_MSG_STR FORMAT "\n", ##__VA_ARGS__)
#define TEST_ERROR_MSG_ERR(FORMAT, ...) std::fprintf(stderr, ERROR_MSG_STR FORMAT "\n", ##__VA_ARGS__)
