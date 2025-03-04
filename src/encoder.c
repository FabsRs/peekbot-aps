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

#include "encoder.h"

int8 encoder_abs_init(PENCODER_ABS encoder_abs)
{
    if(!encoder_abs)
        return -1;

    return 0;
}

int8 encoder_inc_init(PENCODER_INC encoder_inc)
{
    if(!encoder_inc)
        return -1;
    return 0;
}

int8 encoder_abs_read(PENCODER_ABS encoder_abs)
{
    if(!encoder_abs)
        return -1;

    return 0;
}

int8 encoder_inc_read(PENCODER_INC encoder_inc)
{
    if(!encoder_inc)
        return -1;
    int8 state = encoder_inc->state;
    encoder_inc->state = (pinout_pin(encoder_inc->pinA, encoder_inc->maskA) | pinout_pin(encoder_inc->pinB, encoder_inc->maskA) << 1);
    
    return 0;
}
