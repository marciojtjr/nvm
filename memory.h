/**
 * @file memory.h
 *
 */

#include "nvm.h"

#if !defined(__MEMORY_H__)
#define __MEMORY_H__

UInt8 memRead (UInt16 start, UInt8 length, UInt8 *buffRead);
UInt8 memWrite (UInt16 start, UInt8 length, UInt8 *buffWrite);
UInt8 memInit (void);

#endif
