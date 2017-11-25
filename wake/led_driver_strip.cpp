/*
 * Copyright (c) 2017 Dmytro Shestakov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "led_driver.h"

namespace Wk {

const PWMConfig LedDriverFeatures::pwmcfg {
  4000000UL,                                    /* 4MHz PWM clock frequency.   */
  4096,                                         /* Initial PWM period 1ms.      */
  nullptr,
  {
    {PWM_OUTPUT_DISABLED, nullptr},
    {PWM_OUTPUT_DISABLED, nullptr},
    {PWM_OUTPUT_ACTIVE_HIGH, nullptr},
    {PWM_OUTPUT_DISABLED, nullptr}
  },
  0,
  0,
  #if STM32_PWM_USE_ADVANCED
  0
  #endif
};

PWMDriver* const LedDriverFeatures::PWMD{ &PWMD3 };

const uint16_t LedDriverFeatures::LUT[] = {
  0,7,8,9,10,11,13,15,17,19,21,24,27,30,33,36,
  40,44,48,52,56,61,66,71,76,81,86,92,98,104,110,
  116,123,130,137,144,151,159,167,175,183,191,200,
  209,218,227,236,246,256,266,276,286,296,306,316,
  326,337,348,359,370,381,393,405,417,429,441,454,
  467,480,493,506,520,534,548,562,577,592,607,622,
  637,653,669,685,701,717,734,751,768,785,802,819,
  836,853,870,887,983,1110,1240,1400,1571,1750 };

const ioportid_t LedDriverFeatures::pwmPort{ GPIOB };
const uint16_t LedDriverFeatures::pwmPad{ 0 };

} //Wk
