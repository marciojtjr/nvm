#include "..\Unity\src\unity.h"
#include <stdio.h>
#include <setjmp.h>

#include "nvm_tests.h"

void runTest(UnityTestFunction test)
{
    if (TEST_PROTECT())
    {
        test();
    }
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_will_always_pass);
    RUN_TEST(test_manual_initialize_memory);
    RUN_TEST(test_restore_uint8);
    RUN_TEST(test_backup_uint8);
    RUN_TEST(test_backup_read_uint32);
    RUN_TEST(test_backup_read_array_uint8);
    RUN_TEST(test_backup_read_complex_struct);
    RUN_TEST(test_backup_read_simple_struct);
    return UNITY_END();
}
