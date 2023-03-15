#pragma once

#ifdef WIN32
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#else

#include "types.h"
//typedef unsigned char uint8_t;
//typedef signed char int8_t;
//typedef unsigned short uint16_t;
//typedef short int16_t;
//typedef unsigned long uint32_t;
//typedef long int32_t;
//typedef unsigned long long uint64_t;
//typedef long long int64_t;

#ifndef true
#define true TRUE
#define false FALSE
#endif

#endif
