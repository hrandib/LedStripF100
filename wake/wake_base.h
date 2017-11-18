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

// http://www.leoniv.diod.club/articles/wake/wake.html

#ifndef WAKE_BASE_H
#define WAKE_BASE_H

#include "ch_extended.h"
#include "hal.h"
#include "crc8.h"

namespace Wake {

  enum {
    CRC_INIT = 0xDE,
    FEND = 0xC0,    //Frame END
    FESC = 0xDB,    //Frame ESCape
    TFEND = 0xDC,    //Transposed Frame END
    TFESC = 0xDD,    //Transposed Frame ESCape
    DEFAULT_ADDRESS = 7
  };

  enum State {
    WAIT_FEND = 0,     //ожидание приема FEND
    ADDR,     //ожидание приема адреса            //передача адреса
    CMD,      //ожидание приема команды            //передача команды
    NBT,      //ожидание приема количества байт в пакете //передача количества байт в пакете
    DATA     //прием данных               //передача данных
  };

  enum Cmd {
    C_NOP,    //нет операции
    C_ERR,    //ошибка приема пакета
    C_ECHO,    //передать эхо
    C_GETINFO,
    C_SETNODEADDRESS,
    C_GETGROUPADDRESS,
    C_SETGROUPADDRESS = C_GETGROUPADDRESS,
    C_GETOPTIME,
    C_OFF,
    C_ON,
    C_ToggleOnOff,
    C_SAVESETTINGS,
#if BOOTLOADER_EXIST
    C_REBOOT,
#endif
    C_BASE_END
  };

  enum Err {
    ERR_NO, //no error
    ERR_TX, //Rx/Tx error
    ERR_BU, //device busy error
    ERR_RE, //device not ready error
    ERR_PA, //parameters value error
    ERR_NI, //Command not impl
    ERR_NR, //no replay
    ERR_NC, //no carrier
    ERR_ADDRFMT,  //new address is wrong
    ERR_EEPROMUNLOCK //EEPROM wasn't unlocked
  };

  enum DeviceType {
    DevNull,
    DevLedDriver = 0x01,
    DevSwitch = 0x02,
    DevRgbDriver = 0x04,
    DevGenericIO = 0x08,
    DevSensor = 0x10,
    DevPowerSupply = 0x20,
    //    Reserved = 0x40,
    DevCustom = 0x80
  };

  enum AddrType {
    addrGroup,
    addrNode
  };

  class WakeData {
  public:
    static constexpr size_t WAKEDATABUFSIZE = 128;
    struct Packet {
      uint8_t addr;
      uint8_t cmd;
      uint8_t n;
      uint8_t buf[WAKEDATABUFSIZE];
    };
  protected:
    volatile Packet packetData;
    State state;
    uint8_t prevByte;
    Crc::Crc8 crc;
  };

  class WakeBase : public Rtos::BaseStaticThread<256>, WakeData {
  private:
    static void RXerr(UARTDriver* uartp, uartflags_t e);
    static void RXend(UARTDriver* uartp);
    static void RXchar(UARTDriver* uartp, uint16_t c);
    static void TXend1(UARTDriver* uartp);
    static void TXend2(UARTDriver* uartp);
    static void SetDE(UARTDriver* uartp) {
      uartp->usart->CR3 &= ~USART_CR3_DMAR;
      WakeBase& wake = *reinterpret_cast<WakeBase*>(uartp->customData);
      if(wake.portDE_) {
        palSetPad(wake.portDE_, wake.pinDE_);
      }
    }
    static void ClearDE(UARTDriver* uartp) {
      uartp->usart->SR = ~USART_SR_RXNE;
      WakeBase& wake = *reinterpret_cast<WakeBase*>(uartp->customData);
      if(wake.portDE_) {
        palClearPad(wake.portDE_, wake.pinDE_);
      }
      uartp->usart->CR3 |= USART_CR3_DMAR;
    }

    Rtos::ThreadStayPoint stayPoint_;

    UARTDriver* uartd_;
    UARTConfig conf_;

    ioportid_t portDE_;
    uint16_t pinDE_;

    volatile uint8_t ptr;       //data pointer in Rx buffer

    bool CheckNodeAddress()
    {
      uint8_t taddr = packetData.buf[0];
      return taddr == (~packetData.buf[1] & 0xFF)
             && ((taddr && taddr < 80) || (taddr > 112 && taddr < 128));
    }

    bool CheckGroupAddress()
    {
      uint8_t taddr = packetData.buf[0];
      return taddr == (~packetData.buf[1] & 0xFF)
             && taddr > 79 && taddr < 96;
    }

  public:
    WakeBase(UARTDriver& uartd, size_t baud, ioportid_t portDE = 0, uint16_t pinDE = 0) : uartd_{&uartd},
      conf_{ TXend1, TXend2, RXend, RXchar, RXerr, baud, 0, 0, USART_CR3_HDSEL },
      portDE_{portDE}, pinDE_{pinDE}
    {
      uartd.customData = this;
    }
    void Init();
    void main() override;
  };
}//Wake

#endif // WAKE_BASE_H
