#include "nvm.h"
#include "..\Unity\src\unity.h"

FILE *pMemory; //< This is the Flash/EEPROM model
gPNvm_Result gpNvm_err = 0;
void *pRead_len;
void *pRead_val;

void setUp(void)
{
    //Open the file for binary access, create if doesn't exists
    pMemory = fopen(".\\mem.bin", "ab+");
}

void tearDown(void)
{
    fclose(pMemory);
}


void test_restore_uint8(void)
{
    //This is intended to test the reading, so the data will be put there
    // manually, avoiding dependency on other methods
    UInt8 fake_mem[] = {

    };
}

void test_backup_uint8(void)
{
    UInt8 testInt8 = TEST_VALUE_INT8;
    UInt8 *pTestInt8 = &testInt8;

    gpNvm_err = gpNvm_SetAttribute(TEST_8BIT_ID, sizeof(UInt8), (UInt8 *)pTestInt8);
    TEST_ASSERT_FALSE(gpNvm_err);
    gpNvm_err = gpNvm_GetAttribute(TEST_8BIT_ID, (UInt8 *)pRead_len, (UInt8 *)pRead_val);
    TEST_ASSERT_FALSE(gpNvm_err);
    TEST_ASSERT_EQUAL(sizeof(UInt8), *(UInt8 *)pRead_len);
    TEST_ASSERT_EQUAL_INT8(TEST_VALUE_INT8, *(UInt8 *)pRead_val);
}
