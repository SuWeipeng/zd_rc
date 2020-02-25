#include "stm32f1xx_hal.h"
#include "nrf_mavlink.h"
#include "MY_NRF24.h"
#include "usb_device.h"

char myRxData[50];
char myAckPayload[32] = "Ack by firedragon_rc";

vel_target vel;

uint32_t time_stamp;

void update_mavlink(void)
{
  if(HAL_GetTick() - time_stamp > 500){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
  } else {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
  }
  
  if(NRF24_available()) {
    NRF24_read(myRxData, 32);
    uint8_t i;
    mavlink_message_t msg_receive;
    mavlink_status_t mav_status;
    for(i=0; i<32; i++) {
      if(mavlink_parse_char(0, myRxData[i], &msg_receive, &mav_status)) {
        switch (msg_receive.msgid) {
        case MAVLINK_MSG_ID_SIMPLE: {
          mavlink_simple_t packet;
          mavlink_msg_simple_decode(&msg_receive, &packet);
          
          time_stamp = HAL_GetTick();

          /*
          char buffer[32];
          sprintf(buffer, "%d\r\n", packet.data);
          VCPSend((uint8_t *)buffer, strlen(buffer));
          */

          mavlink_message_t msg_ack;
          mavlink_msg_velocity_pack(0, 0, &msg_ack, vel.vel_x, vel.vel_y, vel.rad_z);
          int len = mavlink_msg_to_send_buffer((uint8_t *)myAckPayload, &msg_ack);
          NRF24_writeAckPayload(1, myAckPayload, len);
          VCPSend((uint8_t *)myAckPayload, len);
          break;
        }
        }
      }
    }
  }
  if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_15) == GPIO_PIN_RESET){
    mavlink_message_t msg;
    mavlink_msg_cmd_pack( 0, 0, &msg, 1 );
    int len = mavlink_msg_to_send_buffer((uint8_t *)myAckPayload, &msg);
    NRF24_writeAckPayload(1, myAckPayload, len);
  }
}
