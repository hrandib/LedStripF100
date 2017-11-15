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

#include "ch.h"
#include "hal.h"
//#include "ch_test.h"

#include "shell.h"
#include "chprintf.h"

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

static THD_WORKING_AREA(SHELL_WA_SIZE, 512);

static void cmd_pwm(BaseSequentialStream *chp, int argc, char *argv[]) {
  auto SetPWM = [](pwmcnt_t cnt) {
    pwmEnableChannel(&PWMD3, 2, cnt);
  };
  if(!argc) {
    chprintf(chp, "Usage: pwm\r\n");
    return;
  }
  SetPWM((uint32_t)std::atoi(argv[0]));
}

static const ShellCommand commands[] = {
  {"pwm", cmd_pwm},
  {NULL, NULL}
};

static char histbuf[128];

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD1,
  commands,
  histbuf,
  128
};

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

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

/*
 * Blinker thread, times are in milliseconds.
 */
//static THD_WORKING_AREA(waThread1, 128);
//static __attribute__((noreturn)) THD_FUNCTION(Thread1, arg) {

//  (void)arg;
//  chRegSetThreadName("blinker");
//  while (true) {
//    systime_t time = 500;
//    palClearPad(GPIOB, GPIOB_LED1);
//    chThdSleepMilliseconds(time);
//    palSetPad(GPIOB, GPIOB_LED1);
//    chThdSleepMilliseconds(time);
//  }
//}

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
  chSysInit();

  palSetPadMode(GPIOB, 0, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
  pwmStart(&PWMD3, &pwmcfg);

  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;
  SerialConfig sercfg{115200,
                      USART_CR1_TE | USART_CR1_RE | USART_CR1_UE,
                          0,
                          0};
  sdStart(&SD1, &sercfg);

  /*
   * Shell manager initialization.
   */
  shellInit();

  /*
   * Creates the blinker thread.
   */
//  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(SHELL_WA_SIZE, sizeof(SHELL_WA_SIZE), NORMALPRIO, shellThread, (void*)&shell_cfg1);

//  auto SetPWM = [](pwmcnt_t cnt) {
//    pwmEnableChannel(&PWMD3, 2, cnt);
//  };
  /*
   * Normal main() thread activity, spawning shells.
   */
//  uint8_t cnt{};
  while (true) {
//    SetPWM(cnt++);
    chThdSleepMilliseconds(1000);
  }
}
