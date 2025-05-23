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

#ifndef CCSDS_H
#define CCSDS_H

#include "ccommons.h"

// Structure to represent a CCSDS packet
// version: Version number (3 bits)
// packetType: Packet type (1 bit)
// secHeaderFlag: Secondary header flag (1 bit)
// apid: Application Process Identifier (10 bits)
// payloadFlag: Payload flag (1 bit)
// sequenceFlag: Sequence flag (2 bits)
// packetId: Packet identifier (14 bits)
// dataLength: Length of the data (16 bits)
typedef struct _CCSDS
{
    uint8 version : 3;
    uint8 packetType : 1;
    uint8 secHeaderFlag : 1;
    uint16 apid : 10;
    uint8 payloadFlag : 1;
    uint8 sequenceFlag : 2;
    uint16 packetId : 14;
    uint16 dataLength : 16;
}*PCCSDS,CCSDS;

#endif//CCSDS_H