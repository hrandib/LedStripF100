#ifndef DISPLAY_H
#define DISPLAY_H

#include "ch_extended.h"
#include "ssd1306.h"

extern Rtos::Mailbox<int32_t, 4> dispMsgQueue;

class Display : public Rtos::BaseStaticThread<256>
{
private:
  using Twi = Twis::SoftTwi<Mcudrv::Pb6, Mcudrv::Pb7>;
  using Disp = Mcudrv::ssd1306<Twi, Mcudrv::ssd1306_128x32>;

  void DisplayCurrent(int32_t val)
  {
    char buf[8];
    io::utoa16((uint16_t)val, (uint8_t*)buf);
    Disp::Puts2X(buf, Mcudrv::Resources::font10x16);
    Disp::Puts2X("mA");
  }
  void DisplayBrightness(int32_t val)
  {
    char buf[8];
    buf[0] = ' ';
    io::utoa16((uint16_t)val, (uint8_t*)&buf[1]);
    Disp::Puts2X(buf, Mcudrv::Resources::font10x16);
    Disp::Puts2X("%");
  }
public:
  void Init()
  {
    Twi::Init();
    Disp::Init();
    Disp::Fill();
    Disp::SetContrast(10);
    start(HIGHPRIO - 1);
  }
  void main() final
  {
    int32_t value;
    while(true) {
      msg_t result = dispMsgQueue.fetch(&value, S2ST(20));
      Disp::Fill();
      if(result == MSG_OK) {
        if(value < 0) {
          DisplayCurrent(-value);
        }
        else {
          DisplayBrightness(value);
        }
      }
    }
  }
};

#endif // DISPLAY_H
