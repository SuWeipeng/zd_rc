#ifndef __NRF_MAVLINK_H
#define __NRF_MAVLINK_H

#include <stm32f1xx_hal.h>
#include "mavlink.h"

typedef struct vel_target {
  float vel_x; // m/s
  float vel_y; // m/s
  float rad_z; // rad/s
} vel_target;

void update_mavlink(void);

#endif
