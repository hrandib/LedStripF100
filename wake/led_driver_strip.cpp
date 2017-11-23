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
  0, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
  37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
  53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68,
  69, 70, 71, 72, 73, 74, 75, 82, 91, 102, 114, 128, 144, 162,
  182, 205, 231, 261, 295, 334, 379, 430, 488, 555, 632, 721,
  822, 940, 1075, 1231, 1412, 1621, 1863, 2143, 2469, 2847 };

const ioportid_t LedDriverFeatures::pwmPort{ GPIOB };
const uint16_t LedDriverFeatures::pwmPad{ 0 };

} //Wk
