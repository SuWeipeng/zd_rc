#ifndef __RC_CHANNEL__
#define __RC_CHANNEL__

#include <stm32f1xx_hal.h>

#define ADC_CHANNEL_CNT 4
#define ADC_BUFF_LEN    ADC_CHANNEL_CNT*2
#define ADC_DEAD_ZONE   300
#define ADC_VCP_DEBUG   2

#define ADC_CHANNEL_X_MIN 0
#define ADC_CHANNEL_Y_MIN 30
#define ADC_CHANNEL_Z_MIN 100
#define ADC_CHANNEL_X_MID 2030
#define ADC_CHANNEL_Y_MID 2035
#define ADC_CHANNEL_Z_MID 2207
#define ADC_CHANNEL_X_MAX 4046
#define ADC_CHANNEL_Y_MAX 3800
#define ADC_CHANNEL_Z_MAX 4022

#define VEL_X_MAX_M_S   0.104f
#define VEL_Y_MAX_M_S   0.104f
#define RAD_Z_MAX_RAD_S 0.7f

class RC_Channel
{
public:
  RC_Channel(ADC_HandleTypeDef* hadc);
  ~RC_Channel() {}
  
  void     adc_update(void);
  uint32_t get_value(uint8_t channel);
  float    vel_x(int8_t inv = 1);
  float    vel_y(int8_t inv = 1);
  float    rad_z(int8_t inv = 1);
  float    vel_y_or_z(int8_t inv = 1, int8_t y_or_z = 0);
  
private:
  ADC_HandleTypeDef* _hadc;
  uint32_t           _adc_buf[ADC_BUFF_LEN];
};

#endif /* __RC_CHANNEL__ */
