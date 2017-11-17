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

#include "wake_base.h"
#include "crc8.h"

namespace Wake {

  using namespace Rtos;

  /*
   * This callback is invoked on a receive error, the errors mask is passed
   * as parameter.
   */
  void WakeBase::RXerr(UARTDriver*, uartflags_t)
  {  }

  /*
   * This callback is invoked when a receive buffer has been completely written.
   */
  void WakeBase::RXend(UARTDriver*)
  {  }

  /*
   * This callback is invoked when a character is received but the application
   * was not ready to receive it, the character is passed as parameter.
   */
  void WakeBase::RXchar(UARTDriver* /*uartp*/, uint16_t /*c*/)
  {  }

  /*
   * This callback is invoked when a transmission buffer has been completely
   * read by the driver.
   */
  void WakeBase::TXend1(UARTDriver*)
  {  }

  /*
   * This callback is invoked when a transmission has physically completed.
   */
  void WakeBase::TXend2(UARTDriver* uartp)
  {
    ClearDE(uartp);
  }

  void WakeBase::Init()
  {
    palSetPadMode(uartd_->portDE, uartd_->pinDE, PAL_MODE_OUTPUT_PUSHPULL);

    uartStart(uartd_, &conf_);

    //Starts the transmission, it will be handled entirely in background.

    //    uartStartSend(&UARTD1, 13, "Starting...\r\n");
  }

  void WakeBase::main()
  {
    while(true) {

    }
  }

}//Wake
