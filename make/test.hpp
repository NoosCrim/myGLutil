#pragma once

#include "test_shared.hpp"
#include <string>

#ifndef TEST_NAME
#define TEST_NAME UNNAMED_TEST
#endif

#define TEST_EQUAL(EXPECT, VAL) \
    if(EXPECT != VAL) \
        TEST_FAIL("TEST_EQUAL failed: <" #VAL "> is not equal to <" #EXPECT "> at [" FILE_LINE_STR "]");\
    else\
        TEST_STATUS_MSG("TEST_EQUAL passed: <" #VAL "> is equal to <" #EXPECT "> at [" FILE_LINE_STR "]");\


const char* const test_name_g = STRINGIFY(TEST_NAME);