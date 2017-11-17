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
#ifndef WAKE_BASE_H
#define WAKE_BASE_H

#include "ch_extended.h"
#include "hal.h"

namespace Wake {

  class WakeBase : public Rtos::BaseStaticThread<256>
  {
  private:
    static void RXerr(UARTDriver *uartp, uartflags_t e);
    static void RXend(UARTDriver *uartp);
    static void RXchar(UARTDriver *uartp, uint16_t c);
    static void TXend1(UARTDriver *uartp);
    static void TXend2(UARTDriver *uartp);
    UARTDriver* uartd_;
    UARTConfig conf_;

  public:
    WakeBase(UARTDriver& uartd, size_t baud) : uartd_{&uartd},
      conf_{ TXend1, TXend2, RXend, RXchar, RXerr, baud , 0, 0, USART_CR3_HDSEL }
    {   }
    void Init();
    void main() override;
  };
}//Wake

#endif // WAKE_BASE_H
