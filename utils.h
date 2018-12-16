/**
 * @file utils.h
 * @brief Header file of general utility functions
 *
 * This file contains all the prototypes of general utility functions
 * and must have all other
 *
 * @author Marcio J Teixeira Jr.
 * @date 14/12/18
 *
 */


/**********************************
 * Generic use types
 **********************************
 */
typedef unsigned char UInt8;
typedef signed char Int8;
typedef unsigned short int UInt16;
typedef signed int Int32;
typedef unsigned int UInt32;

/**********************************
 * Prototype of exported functions
 **********************************
 */
UInt16 calcCRC16(UInt8 *buffer, int len);
UInt8 calcCRC8(UInt8 *buffer, int len);
