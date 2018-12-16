/**
 * @file nvm_tests.h
 * @brief Header file of Unit tests module
 *
 * This file contains all the @e defines of testing values, testing addresses,
 * testing data structures and the prototype of the unit test functions.
 *
 * @author Marcio J Teixeira Jr.
 * @date 09/12/18
 *
 */

#if !defined(__NVM_TESTS_H__)
#define __NVM_TESTS_H__

#include "nvm.h"

#define TEST_VALUE_INT8         149
#define TEST_VALUE_INT16        421
#define TEST_VALUE_INT32        135227L
#define TEST_VALUE_STRUCT_ID    98
#define TEST_VALUE_STRUCT_OPT   0xF50014
#define TEST_VALUE_STRUCT_LEN   13

#define TEST_8BIT_MANUAL_START  13483L

#define ARRAY_SIZE      31
#define MAX_STRUCT_DATA_LENGTH 20


#define TEST_8BIT_ID                0x10
#define TEST_16BIT_ID               0x11
#define TEST_32BIT_ID               0x12
#define TEST_8BIT_ARRAY_ID          0x13
#define TEST_16BIT_ARRAY_ID         0x14
#define TEST_STRING_ID              0x15
#define TEST_SIMPLESTRUCT_ID        0x16
#define TEST_COMPLEXSTRUCT_ID       0x17

/**
 * @brief Simples struct to test the reading and writing of non-basic types
 *
 * This is a very simple struct, just to test how the memory
 * access functions deals with struct. It may help to debug, since
 * that is easier to identify and solve errors on simple structs.
 */
typedef struct
{
    UInt8 id;        ///< Some data ID
    UInt32 value32;  ///< A generic 32bit value
    UInt16 value16;  ///< A generic 16bit value
} gpSimpleData_t;

/**
 * @brief Complex struct to test the reading and writing of non-basic types
 *
 * This is the struct suggested for testing on the document.
 */
typedef struct
{
    UInt8 id;                               ///< The data ID
    UInt32 options;                         ///< A map of bits options
    UInt8 length;                           ///< The length of the data array
    UInt8 data[MAX_STRUCT_DATA_LENGTH];     ///< An array of @e length 8 bit values
} gpTestData_t;


//Testing functions
void test_will_always_pass(void);
void test_manual_initialize_memory(void);
void test_restore_uint8(void);
void test_backup_uint8(void);
void test_backup_read_uint32(void);
void test_backup_read_array_uint8(void);
void test_backup_read_simple_struct(void);
void test_backup_read_complex_struct(void);

#endif
