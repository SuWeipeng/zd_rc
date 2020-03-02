#include "nrf_mavlink.h"
#include "RC_Channel.h"
#include "AP_Show.h"
#include "AP_Buffer.h"

extern ADC_HandleTypeDef hadc1;
extern vel_target vel;

RC_Channel* rc;
AP_Show* show;
AP_Buffer *buffer;

#if defined(__GNUC__) && defined(__cplusplus)
extern "C" {
#endif

void setup(void)
{
  rc = new RC_Channel(&hadc1);
  show = new AP_Show();
  show->init(AP_Show::SSD1306_OLED_SPI);
  buffer = new AP_Buffer();
  buffer->init(AP_Buffer::FIFO);
}

void loop(void)
{
  static GPIO_PinState key_last;
  static bool mode;
  static uint32_t time_stamp;
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
  
  if(HAL_GetTick()-time_stamp >= 100){
    show->show_page(1);
    
    // Page 1
    static uint32_t cnt = 0;
    char buf[DISP_MAX_CHAR_PER_LINE];
    char head[DISP_MAX_CHAR_PER_LINE];
    char c[1];
    switch(buffer->get_buf_type()){
    case AP_Buffer::RING:
      sprintf(head, "%s \r\n","ring fifo");
      break;
    case AP_Buffer::FIFO:
      sprintf(head, "%s \r\n","fifo");
      break;
    }
    sprintf(c, "%d", cnt++ % 10);
//    buffer->write(c,sizeof(c)); 
    buffer->write("buffer",6); 
    sprintf(buf, "buf :%s \r\n", (uint8_t*)buffer->get_buffer());
    show->page_write(1, 0, buf, head);
    sprintf(buf, "len :%d \r\n", buffer->buf_len());
    show->page_write(1, 1, buf, head);
    if(cnt%1==0 && buffer->read()>0){
      sprintf(buf, "read:%s \r\n", (uint8_t*)buffer->read_buf_addr());
      show->page_write(1, 2, buf, head);
    }
    
    show->update();
    time_stamp = HAL_GetTick();
  }
}

#if defined(__GNUC__) && defined(__cplusplus)
}
#endif
