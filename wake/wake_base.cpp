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
  void WakeBase::RXchar(UARTDriver* uartp, uint16_t dataByte_)
  {
    WakeBase& wake = *reinterpret_cast<WakeBase*>(uartp->customData);
    uint8_t dataByte = static_cast<uint8_t>(dataByte_);
    if(dataByte == FEND) {
      wake.prevByte = dataByte;
      wake.crc.Reset(CRC_INIT);
      wake.state = ADDR;
      wake.crc(dataByte);
      return;
    }
    if(wake.state == WAIT_FEND) {
      return;
    }
    uint8_t prev = wake.prevByte;               //сохранение старого пре-байта
    wake.prevByte = dataByte;              //обновление пре-байта
    if(prev == FESC) {
      if(dataByte == TFESC)            //а байт данных равен TFESC,
        dataByte = FESC;               //то заменить его на FESC
      else if(dataByte == TFEND)       //если байт данных равен TFEND,
        dataByte = FEND;          //то заменить его на FEND
      else {
        wake.state = WAIT_FEND;     //для всех других значений байта данных,
//          cmd = C_ERR;         //следующего за FESC, ошибка
        return;
      }
    }
    else if(dataByte == FESC) {            //если байт данных равен FESC, он просто
      return;                             //запоминается в пре-байте
    }
    switch(wake.state) {
    case ADDR:                     //-----> ожидание приема адреса
      if(dataByte & 0x80) {
        wake.crc(dataByte); //то обновление CRC и
        dataByte &= 0x7F; //обнуляем бит 7, получаем истинный адрес
        if(dataByte == 0 || dataByte == DEFAULT_ADDRESS/* || dataByte == groupAddr_nv*/) {
          wake.packetData.addr = dataByte;
          wake.state = CMD;       //переходим к приему команды
          break;
        }
        wake.state = WAIT_FEND;        //адрес не совпал, ожидание нового пакета
        break;
      }
      else {
        wake.packetData.addr = 0;	//если бит 7 данных равен нулю, то
      }
      wake.state = CMD;					//сразу переходим к приему команды
    case CMD:                      //-----> ожидание приема команды
      if(dataByte & 0x80) {
        wake.state = WAIT_FEND;        //если бит 7 не равен нулю,
//          cmd = C_ERR;            //то ошибка
        break;
      }
      wake.packetData.cmd = dataByte;          //сохранение команды
      wake.crc(dataByte);				//обновление CRC
      wake.state = NBT;           //переходим к приему количества байт
      break;
    case NBT:
      if(dataByte > WAKEDATABUFSIZE) {
        wake.state = WAIT_FEND;
//          cmd = C_ERR;		//то ошибка
        break;
      }
      wake.packetData.n = dataByte;
      wake.crc(dataByte);		//обновление CRC
      wake.ptr = 0;			//обнуляем указатель данных
      wake.state = DATA;		//переходим к приему данных
      break;
    case DATA:
      if(wake.ptr < wake.packetData.n) {
        wake.packetData.buf[wake.ptr++] = dataByte; //то сохранение байта данных,
        wake.crc(dataByte);  //обновление CRC
        break;
      }
      if(dataByte != wake.crc.GetResult()) {
        wake.state = WAIT_FEND;		//если CRC не совпадает,
//          cmd = C_ERR;			//то ошибка
        break;
      }
      wake.state = WAIT_FEND;		//прием пакета завершен,
      wake.stayPoint_.ResumeFromISR(wake.packetData.cmd);		//загрузка команды на выполнение
      break;
      //warning suppress
    case WAIT_FEND:
      ;
    }
  }

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
    palSetPadMode(portDE_, pinDE_, PAL_MODE_OUTPUT_PUSHPULL);
    palClearPad(portDE_, pinDE_);

    uartStart(uartd_, &conf_);

    start(NORMALPRIO - 1);
  }

  void WakeBase::main()
  {
    while(true) {
      msg_t msg = stayPoint_.Suspend();
      SetDE(uartd_);

      uartStartSend(uartd_, 1, &msg);
    }
  }

}//Wake
