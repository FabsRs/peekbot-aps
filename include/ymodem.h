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

#include "ccommons.h"

#ifndef YMODEM_H
#define YMODEM_H

#define YMODEM_PACKET_SEQNO_INDEX      1
#define YMODEM_PACKET_SEQNO_COMP_INDEX 2
#define YMODEM_PACKET_HEADER           3
#define YMODEM_PACKET_TRAILER          2
#define YMODEM_PACKET_OVERHEAD         PACKET_HEADER + PACKET_TRAILER
#define YMODEM_PACKET_SIZE             128
#define YMODEM_PACKET_1K_SIZE          1024
#define YMODEM_FILE_NAME_LENGTH        256
#define YMODEM_FILE_SIZE_LENGTH        16
#define YMODEM_SOH                     0x01 /* start of 128-byte data packet */
#define YMODEM_STX                     0x02 /* start of 1024-byte data packet */
#define YMODEM_EOT                     0x04 /* end of transmission */
#define YMODEM_ACK                     0x06 /* acknowledge */
#define YMODEM_NAK                     0x15 /* negative acknowledge */
#define YMODEM_CA                      0x18 /* two of these in succession aborts transfer */
#define YMODEM_CRC16                   0x43 /* 'C' == 0x43, request 16-bit CRC */
#define YMODEM_ABORT1                  0x41 /* 'A' == 0x41, abort by user */
#define YMODEM_ABORT2                  0x61 /* 'a' == 0x61, abort by user */
#define YMODEM_NAK_TIMEOUT             5000
#define YMODEM_MAX_ERRORS              5

uint8 ymodem_transmit(uint8* data);
uint8 yomdem_receive(uint8* data, const uint8* filename, uint32 filesize);

#endif//YMODEM_H