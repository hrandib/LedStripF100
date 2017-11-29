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

//  Simplified fully static GPIO control interface

#pragma once

#include "stm32f1xx.h"
#include "type_traits_ex.h"

namespace Mcudrv {
                                  //   CNF MODE ODR
                                  //0b 00  00   0
  enum class GpioModes {
    InputAnalog                   = 0b00'00'0,
    InputFloating                 = 0b01'00'0,
    InputPullup                   = 0b10'00'1,
    InputPulldown                 = 0b10'00'0,
    OutputPushPull                = 0b00'01'0,
    OutputOpenDrain               = 0b01'01'0,
    OutputPushPullAlternate       = 0b10'01'0,
    OutputOpenDrainAlternate      = 0b11'01'0,
    OutputPushPullFast            = 0b00'11'0,
    OutputOpenDrainFast           = 0b01'11'0,
    OutputPushPullFastAlternate   = 0b10'11'0,
    OutputOpenDrainFastAlternate  = 0b11'11'0,
    OutputPushPullSlow            = 0b00'10'0,
    OutputOpenDrainSlow           = 0b01'10'0,
    OutputPushPullSlowAlternate   = 0b10'10'0,
    OutputOpenDrainSlowAlternate  = 0b11'10'0,
  };

  struct GpioBase {
    //compatibility purposes
    enum Cfg {
      In_float = uint8_t(GpioModes::InputFloating),
      In_Pullup = uint8_t(GpioModes::InputPullup),
      Out_OpenDrain = uint8_t(GpioModes::OutputOpenDrain),
      Out_OpenDrain_fast = uint8_t(GpioModes::OutputOpenDrainFast),
      Out_PushPull = uint8_t(GpioModes::OutputPushPull),
      Out_PushPull_fast = uint8_t(GpioModes::OutputPushPullFast)
    };
  };

  template <uintptr_t baseAddr, uint8_t ID>
  class Gpio : GpioBase
  {
  public:
    using DataT = uint16_t;
    enum { Width = 16 };
    enum { id = ID };
  private:
    constexpr static inline GPIO_TypeDef* Regs()
    {
      return reinterpret_cast<GPIO_TypeDef*>(baseAddr);
    }
    static constexpr uint32_t Unpack2Bit(uint32_t mask, uint32_t config, uint32_t value = 0)
    {
      mask = (mask & 0xff00)     << 8 | (mask & 0x00ff);
      mask = (mask & 0x00f000f0) << 4 | (mask & 0x000f000f);
      mask = (mask & 0x0C0C0C0C) << 2 | (mask & 0x03030303);
      mask = (mask & 0x22222222) << 1 | (mask & 0x11111111);
      return (value & ~(mask*0x03)) | mask * config;
    }
    static constexpr uint32_t Unpack4Bit(uint32_t mask, uint32_t config, uint32_t value = 0)
    {
      mask = (mask & 0xf0) << 12 | (mask & 0x0f);
      mask = (mask & 0x000C000C) << 6 | (mask & 0x00030003);
      mask = (mask & 0x02020202) << 3 | (mask & 0x01010101);
      return (value & ~(mask * 0x15)) | mask * config;
    }
  public:
    static void Enable()
    {
      RCC->APB2ENR |= RCC_APB2ENR_IOPAEN << id;
    }
    //constant interface
    template <DataT mask, Cfg config>
    static void SetConfig()
    {
      if(mask & 0xFF) {
        Regs()->CRL = Unpack4Bit(mask, config, Regs()->CRL);
      }
      constexpr DataT maskH = mask >> 8;
      if(maskH) {
        Regs()->CRH = Unpack4Bit(maskH, config, Regs()->CRH);
      }
    }
    template <DataT mask, Cfg config>
    static void WriteConfig()
    {
      if(mask & 0xFF) {
        Regs()->CRL = Unpack4Bit(mask, config);
      }
      constexpr DataT maskH = mask >> 8;
      if(maskH) {
        Regs()->CRH = Unpack4Bit(maskH, config);
      }
    }
    template <DataT mask>

    static void Set()
    {
      Regs()->BSRR = mask;
    }
    template <DataT mask>
    static void Clear()
    {
      Regs()->BRR = mask;
    }
    template<DataT clearmask, DataT setmask>
    static void ClearAndSet()
    {
      const DataT value = setmask & ~clearmask;
      Regs()->ODR = value;
    }
    template <DataT value>
    static void Write()
    {
      Regs()->ODR = value;
    }
    template <DataT mask>
    static void Toggle()
    {
      Regs()->ODR ^= mask;
    }

    //normal interface
    static void SetConfig(DataT mask, Cfg config)
    {
      if(mask & 0xFF) {
        Regs()->CRL = Unpack4Bit(mask, config, Regs()->CRL);
      }
      mask >>= 8;
      if(mask) {
        Regs()->CRH = Unpack4Bit(mask, config, Regs()->CRH);
      }
    }
    static void WriteConfig(DataT mask, Cfg config)
    {
      if(mask & 0xFF) {
        Regs()->CRL = Unpack4Bit(mask, config);
      }
      mask >>= 8;
      if(mask) {
        Regs()->CRH = Unpack4Bit(mask, config);
      }
    }
    static void Set(DataT mask)
    {
      Regs()->BSRR = mask;
    }
    static void Clear(DataT mask)
    {
      Regs()->BRR = mask;
    }
    static void ClearAndSet(DataT clearmask, DataT setmask)
    {
      const DataT value = setmask & ~clearmask;
      Regs()->ODR = value;
    }
    static void Write(DataT value)
    {
      Regs()->ODR = value;
    }
    static void Toggle(DataT mask)
    {
      Regs()->ODR ^= mask;
    }
    static DataT Read()
    {
      return Regs()->IDR;
    }
    static DataT ReadODR()
    {
      return Regs()->ODR;
    }
  };

  struct GpioNull : GpioBase {
    using DataT = uint16_t;
    enum { Width = 16 };
    enum { id = 0xFF };
    template <DataT mask, Cfg cfg>
    static void SetConfig()
    { }
    template <DataT mask, Cfg cfg>
    static void WriteConfig()
    { }
    template <DataT value>
    static void Write()
    { }
    static void Write(DataT /*value*/)
    { }
    template <DataT mask>
    static void Set()
    { }
    static void Set(DataT /*mask*/)
    { }
    template <DataT mask>
    static void Clear()
    { }
    static void Clear(DataT /*mask*/)
    { }
    template <DataT mask>
    static void Toggle()
    { }
    static void Toggle(DataT /*mask*/)
    { }
    template<DataT clearmask, DataT setmask>
    static void ClearAndSet()
    { }
    static DataT Read()
    {
      return 0;
    }
    static DataT ReadODR()
    {
      return 0;
    }
  };

  template<typename... Ports>
  static void PortEnable()
  {
    static_assert(((Ports::id < 5) && ...), "Wrong port ID");
    RCC->APB2ENR |= ((RCC_APB2ENR_IOPAEN << Ports::id) | ...);
  }
  template<typename... Ports>
  static void PortEnable(Ports...)
  {
    PortEnable<Ports...>();
  }

#define PORTDEF(x,y) typedef Gpio<GPIO##x##_BASE, y> Gpio##x

  PORTDEF(A, 0);
  PORTDEF(B, 1);
  PORTDEF(C, 2);
  PORTDEF(D, 3);
  PORTDEF(E, 4);

  template <typename Port_, uint16_t Mask_>
  class TPin
  {
  public:
    typedef Port_ Port;
    enum {
      mask = Mask_,
      position = Utils::MaskToPosition<mask>::value,
      port_id = Port::id
    };
    static const bool Exist = mask;
    template <GpioBase::Cfg cfg>
    static void SetConfig()
    {
      Port::template SetConfig<mask, cfg>();
    }
    static void Set()
    {
      Port::template Set<mask>();
    }
    static void SetOrClear(bool cond)
    {
      if(cond) {
        Port::template Set<mask>();
      }
      else {
        Port::template Clear<mask>();
      }
    }
    static void Clear()
    {
      Port::template Clear<mask>();
    }
    static void Toggle()
    {
      Port::template Toggle<mask>();
    }
    static bool IsSet()
    {
      return Port::Read() & mask;
    }
    static bool IsODRSet()
    {
      return Port::ReadODR() & mask;
    }
  };

  template<typename Pin>
  struct InvertedPin : public Pin {
    static void Set()
    {
      Pin::Clear();
    }
    static void Clear()
    {
      Pin::Set();
    }
    static bool IsSet()
    {
      return !Pin::IsSet();
    }
    static bool IsODRSet()
    {
      return !Pin::IsODRSet();
    }
  };

#define PINSDEF(x, y) \
  			typedef TPin<Gpio##x, 0x0001> P##y##0;\
            typedef TPin<Gpio##x, 0x0002> P##y##1;\
            typedef TPin<Gpio##x, 0x0004> P##y##2;\
            typedef TPin<Gpio##x, 0x0008> P##y##3;\
            typedef TPin<Gpio##x, 0x0010> P##y##4;\
            typedef TPin<Gpio##x, 0x0020> P##y##5;\
            typedef TPin<Gpio##x, 0x0040> P##y##6;\
            typedef TPin<Gpio##x, 0x0080> P##y##7;\
            typedef TPin<Gpio##x, 0x0100> P##y##8;\
            typedef TPin<Gpio##x, 0x0200> P##y##9;\
            typedef TPin<Gpio##x, 0x0400> P##y##10;\
            typedef TPin<Gpio##x, 0x0800> P##y##11;\
            typedef TPin<Gpio##x, 0x1000> P##y##12;\
            typedef TPin<Gpio##x, 0x2000> P##y##13;\
            typedef TPin<Gpio##x, 0x4000> P##y##14;\
            typedef TPin<Gpio##x, 0x8000> P##y##15;
  PINSDEF(A, a)
  PINSDEF(B, b)
  PINSDEF(C, c)
  PINSDEF(D, d)
  PINSDEF(E, e)
  typedef TPin<GpioNull, 0x0> Nullpin;

  enum {
    P0  = 0x0001,
    P1  = 0x0002,
    P2  = 0x0004,
    P3  = 0x0008,
    P4  = 0x0010,
    P5  = 0x0020,
    P6  = 0x0040,
    P7  = 0x0080,
    P8  = 0x0100,
    P9  = 0x0200,
    P10 = 0x0400,
    P11 = 0x0800,
    P12 = 0x1000,
    P13 = 0x2000,
    P14 = 0x4000,
    P15 = 0x8000
  };
} //Mcudrv
