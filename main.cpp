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

using namespace Rtos;

using LedDriver = Wk::LedDriver<>;

static Wk::Wake<LedDriver> wake(UARTD1, 115200, GPIOA, 10);

class ButtonControl {
private:
  ioportid_t buttonPort_;
  uint16_t buttonPin_;
  uint8_t direction{};
  uint8_t count{};
public:
  ButtonControl(ioportid_t port, uint16_t pin) : buttonPort_{port}, buttonPin_{pin}
  {
    palSetPadMode(port, pin, PAL_MODE_INPUT_PULLDOWN);
  }
  void Update()
  {
    if(palReadPad(buttonPort_, buttonPin_)) {
      ++count;
      if(count > 15 && count & 0x01) {
        if(!direction) {
          LedDriver::Increment(1);
        }
        else {
          LedDriver::Decrement(1);
        }
      }
    }
    else if(count) {
      if(count >= 2 && count <= 15) {
        LedDriver::ToggleOnOff();
      }
      else {
        direction ^= 1;
      }
      count = 0;
    }
  }
};

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

  wake.Init();
  ButtonControl buttonControl{GPIOB, 10};
  while (true) {
    buttonControl.Update();
    BaseThread::sleep(MS2ST(16));
  }
}
