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

extern Rtos::Mailbox<int32_t, 4> dispMsgQueue;

//only one instance is allowed
class Measure : public Rtos::BaseStaticThread<256>
{
private:
  static constexpr size_t channelsNum = 1;
  static constexpr size_t bufDepth = 16;
  static adcsample_t samples[channelsNum * bufDepth];
  static Rtos::Mailbox<int32_t, 4> adcMeasQueue;

  const ADCConversionGroup adcGroupCfg = {
    true, //is circular
    channelsNum,
    AdcCb,
    nullptr, //ERR cb
    0, 0,                         /* CR1, CR2 */
    0,                            /* SMPR1 */
    ADC_SMPR2_SMP_AN4(ADC_SAMPLE_239P5),
    ADC_SQR1_NUM_CH(channelsNum),
    0,                            /* SQR2 */
    ADC_SQR3_SQ1_N(ADC_CHANNEL_IN4)
  };

  static void AdcCb(ADCDriver* /*adcp*/, adcsample_t* buffer, size_t n) {
    int32_t result{};
    if(buffer == samples) while(n) {
      result += buffer[--n];
    }
    Rtos::SysLockGuardFromISR lock;
    adcMeasQueue.postI(result);
  }

public:
  void Init()
  {
    palSetPadMode(GPIOA, 4, PAL_MODE_INPUT_ANALOG);
    adcStart(&ADCD1, nullptr);
    adcStartConversion(&ADCD1, &adcGroupCfg, samples, bufDepth);
  }
  void main() final
  {
    while(true) {
      int32_t result;
      msg_t status = adcMeasQueue.fetch(&result, TIME_INFINITE);
      if(status == MSG_OK) {
        (void)result;
      }
    }

  }

};

#endif // MEASURE_H
