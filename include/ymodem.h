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

#ifndef YMODEM_H
#define YMODEM_H

#include "ccommons.h"

#define SOH 0x01
#define STX 0x02
#define EOT 0x04
#define ACK 0x06
#define NAK 0x15
#define CAN 0x18
#define CRC16_POLY 0x1021

// Enum to represent YMODEM status
// YMODEM_OK: Operation successful
// YMODEM_ERROR: Operation failed
// YMODEM_TIMEOUT: Operation timed out
typedef enum {
    YMODEM_OK,
    YMODEM_ERROR,
    YMODEM_TIMEOUT
} ymodem_status;

// Structure to represent a YMODEM file
// data: Pointer to the file data
// size: Size of the file
// filename: Name of the file
typedef struct _YMODEM_FILE{
    const uint8* data;
    uint32 size;
    char filename[256];
}*PYMODEM_FILE, YMODEM_FILE;

// Calculates the CRC16 checksum for the given data
// data: Pointer to the data
// size: Size of the data
// returns: CRC16 checksum
uint16 ymodem_crc16(const uint8* data, uint16 size);

#endif//YMODEM_H