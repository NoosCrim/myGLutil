#include <cstdio>
#include "test_shared.hpp"

class test_fail_exception;
int main()
{
    try
    {
        test_function();
    }
    catch(const test_fail_exception &exc)
    {
        TEST_FAIL_MSG("%s failed: %s\n", test_name_g, exc.what());
        TEST_FAIL_MSG_ERR("%s failed: %s\n", test_name_g, exc.what());
        return 1;
    }
    catch(const std::exception &exc)
    {
        TEST_ERROR_MSG("%s stopped because of  external exception: %s\n", test_name_g, exc.what());
        TEST_ERROR_MSG_ERR("%s stopped because of  external exception: %s\n", test_name_g, exc.what());
        return 1;
    }
    catch(...)
    {
        TEST_ERROR_MSG("%s stopped because of unknown exception", test_name_g);
        TEST_ERROR_MSG_ERR("%s stopped because of  unknown exception", test_name_g);
        return 1;
    }
    
}