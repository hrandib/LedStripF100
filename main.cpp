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
#include "led_driver.h"
#include "button_control.h"
#include "display.h"

using namespace Rtos;

using LedDriver = Wk::LedDriver<>;

using ButtonControl = Wk::ButtonControl<LedDriver>;

using Twi = Twis::SoftTwi<Mcudrv::Pb6, Mcudrv::Pb7>;
using Disp = Mcudrv::ssd1306<Twi>;

static Wk::Wake<LedDriver> wake(UARTD1, 115200, GPIOA, 10);

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
//  System::init();
  using namespace Mcudrv;
  GpioB::Enable();
//  wake.Init();
  Twi::Init();
  Disp::Init();
  Disp::Fill(Mcudrv::Color::Solid);
  Disp::Puts("Hello ");
  Disp::Puts("Hello ");
//  GpioB::SetConfig<P6 | P7, GpioBase::Cfg(GpioModes::OutputOpenDrain)>();
//  GpioB::Set(P6 | P7);
//  Pb0::SetConfig<GpioBase::Cfg(GpioModes::OutputPushPull)>();
//  ButtonControl buttonControl{GPIOB, 10};
  while (true) {
//    Pb0::Set();
    volatile size_t count = 100000;
    while(count--) { }
    Pb0::Clear();
    count = 100000;
    while(count--) { }
//    buttonControl.Update();
//    BaseThread::sleep(buttonControl.GetUpdatePeriod());
  }
}
