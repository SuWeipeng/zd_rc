#include "nrf_mavlink.h"
#include "RC_Channel.h"

extern ADC_HandleTypeDef hadc1;
extern vel_target vel;

RC_Channel* rc;

#if defined(__GNUC__) && defined(__cplusplus)
extern "C" {
#endif

void setup(void)
{
  rc = new RC_Channel(&hadc1);
}

void loop(void)
{
  vel.vel_x = rc->vel_x();
  vel.vel_y = rc->vel_y(-1);
  vel.rad_z = rc->rad_z(-1);
}

#if defined(__GNUC__) && defined(__cplusplus)
}
#endif
