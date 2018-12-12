/**
 * @file memory.h
 *
 */

#if !defined(__MEMORY_H__)
#define __MEMORY_H__

Int8 memRead (UInt16 start, UInt8 length, UInt8 *buffRead);
Int8 memWrite (UInt16 start, UInt8 length, UInt8 *buffWrite);
Int8 memInit (void);
