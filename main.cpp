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

#include <stdio.h>
#include <string.h>
#include <cstdlib>

#include "ch_extended.h"
#include "hal.h"
#include "chprintf.h"
#include "wake_base.h"

using namespace Rtos;

static PWMConfig pwmcfg = {
  4000000UL,                                    /* 10kHz PWM clock frequency.   */
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

struct Module : Wk::NullModule {
  static void On() {
    pwmEnableChannel(&PWMD3, 2, 50);
  }
  static void Off() {
    pwmEnableChannel(&PWMD3, 2, 0);
  }
};

static Wk::Wake<Module> wake(UARTD1, 115200, GPIOA, 10);

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  System::init();


  palSetPadMode(GPIOB, 0, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
  pwmStart(&PWMD3, &pwmcfg);
  wake.Init();

  while (true) {
//    pwmEnableChannel(&PWMD3, 2, 10);
//    BaseThread::sleep(MS2ST(2200));
//    pwmEnableChannel(&PWMD3, 2, 0);
    BaseThread::sleep(MS2ST(2100));
  }
}
