/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
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
