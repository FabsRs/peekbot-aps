#ifndef CCOMMONS_H
#define CCOMMONS_H 1

#include <stdio.h>
#include <stdlib.h>

/* ARRAYS */
#define varchar(size, var) char var[size]
#define STR8                8
#define STR16               16
#define STR32               32
#define STR64               64
#define STR128              128
#define STR256              256
#define STR1K               1024

/* TYPES */
typedef unsigned char       uint8;
typedef unsigned short int  uint16;
typedef unsigned int        uint32;
typedef unsigned long int   uint64;
typedef char                int8;
typedef short int           int16;
typedef int                 int32;
typedef long int            int64;
typedef uint8*              puint8;
typedef uint16*             puint16;
typedef uint32*             puint32;
typedef uint64*             puint64;
typedef int8*               pint8;
typedef int16*              pint16;
typedef int32*              pint32;
typedef int64*              pint64;

/* ERROR HANDLERS */
#define __err_exit(str, errno, exit_status) do{exit(exit_status);}while(0)
#define __err_return_int(str, errno, status) do{return status;}while(0)
#define __err_return_null(str, errno) do{return NULL;}while(0)
#define __err_return_goto  (str, errno, GOTO) do{goto GOTO;}while(0)
#define __err_debug_exit() do{exit(0);}while(0)

/* FUNCTION PARAMETER CHECK */
#define __pn(param)do{if(!param)__err_return_null("Null parameter passed as argument", errno);}while(0)
#define __pi(param)do{if(!param)__err_return_int("Null parameter passed as argument", errno, -1);}while(0)

/* FUNCTION WRAPPER */
#define __fpp(func, var, ...)do{if(!(var = func(__VA_ARGS__)))__err_return_null("Function call failed", errno);}while(0)
#define __fip(func, var, ...)do{if(!(var = func(__VA_ARGS__)))__err_return_int("Function call failed", errno, -1);}while(0)
#define __fp(func, ...)do{if(func(__VA_ARGS__))__err_return_null("Function call failed", errno);}while(0)
#define __fi(func, ...)do{if(func(__VA_ARGS__))__err_return_int("Function call failed", errno, -1);}while(0)

#endif//CCOMMONS_H