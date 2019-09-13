#include <AP_Math.h>
#include "usb_device.h"
#include "RC_Channel.h"
#include "ssd1306.h"

#define CHECK_SIGN(x,y) (((x>0.0f&&y>0.0f) || (x<0.0f&&y<0.0f)) ? 1 : 0)

RC_Channel::RC_Channel(ADC_HandleTypeDef* hadc)
  : _hadc(hadc)
{
  HAL_ADC_Start_DMA(_hadc, _adc_buf, ADC_BUFF_LEN);
}

uint32_t RC_Channel::get_value(uint8_t channel)
{
  if(channel > ADC_CHANNEL_CNT -1) return 0;
#if ADC_VCP_DEBUG == 2  
  char buffer[30];
  sprintf(buffer, "x:%d, y:%d, z:%d\r\n", _adc_buf[3], _adc_buf[0], _adc_buf[2]);
  VCPSend((uint8_t *)buffer, strlen(buffer));
#endif  
  return _adc_buf[channel];
}

float RC_Channel::vel_x(int8_t inv)
{
  float    ret = 0.0f;
  uint16_t min = ADC_CHANNEL_X_MIN;
  uint16_t mid = ADC_CHANNEL_X_MID;
  uint16_t max = ADC_CHANNEL_X_MAX;
  uint16_t val = get_value(3);
  uint16_t mid_zone_up   = mid + ADC_DEAD_ZONE;
  uint16_t mid_zone_down = mid - ADC_DEAD_ZONE;

  if(abs(val-mid) < ADC_DEAD_ZONE){
    ret = 0.0f;
  } else if(val > mid) {
    ret = (val-mid_zone_up)*(VEL_X_MAX_M_S/(max-mid_zone_up));
  } else {
    ret = (val-mid_zone_down)*(VEL_X_MAX_M_S/(mid_zone_down-min));
  }
  
  if(inv == -1) ret *= inv;

  ret = constrain_float(ret, -VEL_X_MAX_M_S, VEL_X_MAX_M_S);
  
  char buffer[30];
  sprintf(buffer, "vel_x:%.3f m/s\r\n", ret);  
#if ADC_VCP_DEBUG == 1  
  VCPSend((uint8_t *)buffer, strlen(buffer));
#endif 
  static float last_ret = ret;
  if((last_ret != ret) && CHECK_SIGN(last_ret, ret) == 0){
    ssd1306_Fill(Black);
    last_ret = ret;
  }
  ssd1306_SetCursor(2, 18*0);
  ssd1306_WriteString(buffer, Font_7x10, White);
  ssd1306_UpdateScreen();
  
  return ret;
}

float RC_Channel::vel_y(int8_t inv)
{
  float    ret = 0.0f;
  uint16_t min = ADC_CHANNEL_Y_MIN;
  uint16_t mid = ADC_CHANNEL_Y_MID;
  uint16_t max = ADC_CHANNEL_Y_MAX;
  uint16_t val = get_value(0);
  uint16_t mid_zone_up   = mid + ADC_DEAD_ZONE;
  uint16_t mid_zone_down = mid - ADC_DEAD_ZONE;

  if(abs(val-mid) < ADC_DEAD_ZONE){
    ret = 0.0f;
  } else if(val > mid) {
    ret = (val-mid_zone_up)*(VEL_Y_MAX_M_S/(max-mid_zone_up));
  } else {
    ret = (val-mid_zone_down)*(VEL_Y_MAX_M_S/(mid_zone_down-min));
  }
  
  if(inv == -1) ret *= inv;

  ret = constrain_float(ret, -VEL_Y_MAX_M_S, VEL_Y_MAX_M_S);
  
  char buffer[30];
  sprintf(buffer, "vel_y:%.3f m/s\r\n", ret);  
#if ADC_VCP_DEBUG == 1  
  VCPSend((uint8_t *)buffer, strlen(buffer));
#endif  
  static float last_ret = ret;
  if((last_ret != ret) && CHECK_SIGN(last_ret, ret) == 0){
    ssd1306_Fill(Black);
    last_ret = ret;
  }
  ssd1306_SetCursor(2, 18*1);
  ssd1306_WriteString(buffer, Font_7x10, White);
  ssd1306_UpdateScreen();  
  
  return ret;
}

float RC_Channel::rad_z(int8_t inv)
{
  float    ret = 0.0f;
  uint16_t min = ADC_CHANNEL_Z_MIN;
  uint16_t mid = ADC_CHANNEL_Z_MID;
  uint16_t max = ADC_CHANNEL_Z_MAX;
  uint16_t val = get_value(2);
  uint16_t mid_zone_up   = mid + ADC_DEAD_ZONE;
  uint16_t mid_zone_down = mid - ADC_DEAD_ZONE;

  if(abs(val-mid) < ADC_DEAD_ZONE){
    ret = 0.0f;
  } else if(val > mid) {
    ret = (val-mid_zone_up)*(RAD_Z_MAX_RAD_S/(max-mid_zone_up));
  } else {
    ret = (val-mid_zone_down)*(RAD_Z_MAX_RAD_S/(mid_zone_down-min));
  }
  
  if(inv == -1) ret *= inv;

  ret = constrain_float(ret, -RAD_Z_MAX_RAD_S, RAD_Z_MAX_RAD_S);
  
  char buffer[30];
  sprintf(buffer, "rad_z:%.3frad/s\r\n", ret);  
#if ADC_VCP_DEBUG == 1  
  VCPSend((uint8_t *)buffer, strlen(buffer));
#endif 
  static float last_ret = ret;
  if((last_ret != ret) && CHECK_SIGN(last_ret, ret) == 0){
    ssd1306_Fill(Black);
    last_ret = ret;
  }
  ssd1306_SetCursor(2, 18*2);
  ssd1306_WriteString(buffer, Font_7x10, White);
  ssd1306_UpdateScreen(); 
  
  return ret;
}

float RC_Channel::vel_y_or_z(int8_t inv, int8_t y_or_z)
{
  float    ret   = 0.0f;
  float    limit = 0.0f;
  uint16_t min   = ADC_CHANNEL_Z_MIN;
  uint16_t mid   = ADC_CHANNEL_Z_MID;
  uint16_t max   = ADC_CHANNEL_Z_MAX;
  uint16_t val   = get_value(2);
  uint16_t mid_zone_up   = mid + ADC_DEAD_ZONE;
  uint16_t mid_zone_down = mid - ADC_DEAD_ZONE;

  switch(y_or_z){
  case 0:{  // vel_y
    min   = ADC_CHANNEL_Y_MIN;
    mid   = ADC_CHANNEL_Y_MID;
    max   = ADC_CHANNEL_Y_MAX;
    limit = VEL_Y_MAX_M_S;
    break;
  }
  case 1:{  // rad_z
    min   = ADC_CHANNEL_Z_MIN;
    mid   = ADC_CHANNEL_Z_MID;
    max   = ADC_CHANNEL_Z_MAX;
    limit = RAD_Z_MAX_RAD_S;
    break;
  }
  }
  mid_zone_up   = mid + ADC_DEAD_ZONE;
  mid_zone_down = mid - ADC_DEAD_ZONE;
  
  if(abs(val-mid) < ADC_DEAD_ZONE){
    ret = 0.0f;
  } else if(val > mid) {
    ret = (val-mid_zone_up)*(limit/(max-mid_zone_up));
  } else {
    ret = (val-mid_zone_down)*(limit/(mid_zone_down-min));
  }
  
  if(inv == -1) ret *= inv;

  ret = constrain_float(ret, -limit, limit);
  
  char buffer[30];
  switch(y_or_z){
  case 0:{
    sprintf(buffer, "vel_y:%.3f m/s\r\n", ret);  
    break;
  }
  case 1:{
    sprintf(buffer, "rad_z:%.3frad/s\r\n", ret);  
    break;
  }
  }
  
#if ADC_VCP_DEBUG == 1  
  VCPSend((uint8_t *)buffer, strlen(buffer));
#endif 
  static float last_ret = ret;
  if((last_ret != ret) && CHECK_SIGN(last_ret, ret) == 0){
    ssd1306_Fill(Black);
    last_ret = ret;
  }
  ssd1306_SetCursor(2, 18*2);
  ssd1306_WriteString(buffer, Font_7x10, White);
  ssd1306_UpdateScreen(); 
  
  return ret;
}
