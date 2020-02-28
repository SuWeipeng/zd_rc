#include "nrf_mavlink.h"
#include "RC_Channel.h"
#include "AP_Show.h"

extern ADC_HandleTypeDef hadc1;
extern vel_target vel;

RC_Channel* rc;
AP_Show* show;

#if defined(__GNUC__) && defined(__cplusplus)
extern "C" {
#endif

void setup(void)
{
  rc = new RC_Channel(&hadc1);
  show = new AP_Show();
  show->init(AP_Show::SSD1306_OLED_SPI);
}

void loop(void)
{
  static GPIO_PinState key_last;
  static bool mode;
  GPIO_PinState key = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10);
  
  vel.vel_x = rc->vel_x();
  vel.vel_y = rc->vel_y(-1);
  
  if(key_last != key){
    if(key == GPIO_PIN_RESET){
      mode = !mode;
    }
    key_last =  key;
  }
  
  if(mode == true){
    vel.vel_y = rc->vel_y_or_z(-1, 0);
    vel.rad_z = 0.0f;
  } else {
    vel.rad_z = rc->vel_y_or_z(-1, 1);
  }
  
  show->update();
}

#if defined(__GNUC__) && defined(__cplusplus)
}
#endif
