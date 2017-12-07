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
#ifndef MEASURE_H
#define MEASURE_H

#include "hal.h"
#include "ch_extended.h"

#include <numeric>
#include <iterator>

extern Rtos::Mailbox<int32_t, 4> dispMsgQueue;

//only one instance is allowed
class Measure
{
private:
  static constexpr size_t samplingTime = MS2ST(100);
  static constexpr size_t channelsNum = 1;
  static constexpr size_t bufDepth = 16;
  static adcsample_t samples[channelsNum * bufDepth];
  static const ADCConversionGroup adcGroupCfg;
  static virtual_timer_t convertVt;

  static int32_t ConvertTomA(int32_t value)
  {
    return value / 20;
  }
  static void AdcCb(ADCDriver* /*adcp*/, adcsample_t* buffer, size_t n) {
    int32_t result{};
    while(n) {
      result += buffer[--n];
    }
    result = ConvertTomA(result);
    if(result > 20) {
      Rtos::SysLockGuardFromISR lock;
      dispMsgQueue.postI(-result);
    }
  }
  static void TimerCb(void*)
  {
    adcStartConversion(&ADCD1, &adcGroupCfg, samples, bufDepth);
    Rtos::SysLockGuardFromISR lock;
    chVTSetI(&convertVt, samplingTime, TimerCb, nullptr);
  }
public:
  static void Init()
  {
    palSetPadMode(GPIOA, 4, PAL_MODE_INPUT_ANALOG);
    adcStart(&ADCD1, nullptr);
    chVTObjectInit(&convertVt);
    adcStartConversion(&ADCD1, &adcGroupCfg, samples, bufDepth);
    chVTSet(&convertVt, samplingTime, TimerCb, nullptr);
  }
};

#endif // MEASURE_H
