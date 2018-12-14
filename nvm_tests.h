/**
 * @file nvm_tests.h
 *
 */

#if !defined(__NVM_TESTS_H__)
#define __NVM_TESTS_H__



#define TEST_VALUE_INT8     149
#define TEST_VALUE_INT16    421
#define TEST_VALUE_INT32    135227L

#define TEST_8BIT_MANUAL_START  13483L




// Old stuff
#define TEST_8BIT_ID                0x10
#define TEST_16BIT_ID               0x11
#define TEST_32BIT_ID               0x12
#define TEST_16BIT_ARRAY_ID         0x13
#define TEST_STRING_ID              0x14
#define TEST_SIMPLESTRUCT_ID        0x15
#define TEST_COMPLEXSTRUCT_ID       0x16

#define UINT8_TEST                  0x01
#define UINT16_TEST                 0x02
#define UINT32_TEST                 0x04
#define UINT16_ARRAY_TEST           0x08
#define STRING_TEST                 0x10
#define SIMPLESTRUCT_TEST           0x20
#define COMPLEXSTRUCT_TEST          0x40


//Testing functions
void test_will_always_pass(void);
void test_manual_initialize_memory(void);
void test_restore_uint8(void);
void test_backup_uint8(void);


#endif
