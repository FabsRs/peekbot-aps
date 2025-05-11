// MIT License
// 
// Copyright (c) 2024 Fabrizio Eduardo Rojas Iacocca
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the SWoftware is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef CCOMMONS_H
#define CCOMMONS_H

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
#define STR512              512
#define STR1K               1024

/* TYPES */
typedef unsigned char       uint8;
typedef unsigned int        uint16;
typedef unsigned long       uint32;
typedef char                int8;
typedef int                 int16;
typedef long                int32;
typedef uint8*              puint8;
typedef uint16*             puint16;
typedef uint32*             puint32;
typedef int8*               pint8;
typedef int16*              pint16;
typedef int32*              pint32;

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