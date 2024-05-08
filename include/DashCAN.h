#ifndef DASHCAN_H
#define DASHCAN_H

#include<Arduino.h>
#include<FlexCAN_T4.h>
#include <OLED.h>
#include <StateVars.h>
using u32 = uint32_t;
using u8 = uint8_t;

#define INCREMENT_P_GAIN 0.002
#define INCREMENT_D_GAIN 0.001
#define INCREMENT_HIGH_TARGET 50.0
#define INCREMENT_LOW_TARGET 50.0



class DashCAN{
public:
    
//RECIEVING
  static const u32 EG_RPM = 0x000;
  static const u32 WHEEL_RPM = 0x001;
  static const u32 ACTUATOR_POS = 0x002;
  static const u32 TARGET_RPM = 0x00f;
  
//SENDING & RECIEVING
  static const u32 LOW_SP_TARG_RPM = 0x003;
  static const u32 HI_SP_TARG_RPM = 0x004;
  static const u32 P_GAIN = 0x005;
  static const u32 D_GAIN = 0x006;
//SENDING
  static const u32 BUTTON1 = 0x007;
  static const u32 BUTTON2 = 0x009;
  static const u32 BUTTON3 = 0x00b;
  static const u32 BUTTON4 = 0x00c;
  static const u32 BUTTON5 = 0x00d;
  static const u32 CLUTCH = 0x00e;
  static const u32 CONTROLLER_TOGGLE = 0x010;



  static const u8 CMD_SUCCESS = 0;
  static const u8 CMD_ERROR_INVALID_AXIS = 1;
  static const u8 CMD_ERROR_INVALID_COMMAND = 2;
  static const u8 CMD_ERROR_WRITE_FAILED = 3;
    
  //bool IDLE = false;
  double last_CAN_msg = 0;
    DashCAN(FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> *flexcan_bus, u32 node_id, StateVars* state, OLED* oledPtr);
    void parse_message(const CAN_message_t &msg);
    u8 set_p_gain(float p_gain);
    u8 set_d_gain(float d_gain);
    u8 set_low_target(float target_rpm);
    u8 set_high_target(float target_rpm);
    u8 send_clutch(bool clutch_status);
    u8 send_controller_toggle(bool controller_toggle);
    StateVars increments;

private:
  FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> *flexcan_bus;
  u32 node_id;
  StateVars* state;
  u8 send_command(u32 cmd_id, bool remote, u8 buf[8]);
  OLED* oledPtr;

};


#endif