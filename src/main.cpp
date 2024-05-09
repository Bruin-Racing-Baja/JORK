#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <string>
#include <FlexCAN_T4.h>
#include <DashCAN.h>
#include <OLED.h>

#define DASH_NODE_ID 0x04
#define BUTTON1 2
#define BUTTON2 3
#define BUTTON3 4
#define BUTTON4 5
#define BUTTON5 6
#define DEBOUNCE_TIME 100

constexpr u32 FLEXCAN_BAUD_RATE = 250000UL;
constexpr u32 FLEXCAN_MAX_MAILBOX = 16UL;


//Creating the important objects for the dashboard

FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> flexcan_bus;
StateVars state;
OLED oled;



DashCAN can(&flexcan_bus, DASH_NODE_ID, &state, &oled);
double button1_debounce_time = 0;
double button2_debounce_time = 0;
double button3_debounce_time = 0;
double button4_debounce_time_rising = 0;
double button4_debounce_time_falling = 0;
double button5_debounce_time_rising = 0;
double button5_debounce_time_falling = 0;


// Creating ISR functions
void right_button_isr(){

  if(millis()-button2_debounce_time > DEBOUNCE_TIME){
    //Serial.println("right");
    switch(oled.get_selection()){
      case P_GAIN_SELECT:
        can.set_p_gain(state.p + can.increments.p);
        break;
      case D_GAIN_SELECT:
        can.set_d_gain(state.d + can.increments.d);
        break;
      case LOW_TARGET:
        can.set_low_target(state.low_target + can.increments.low_target);
        break;
      case HIGH_TARGET:
        can.set_high_target(state.high_target + can.increments.high_target);
        //Serial.println(state.high_target + can.increments.high_target);
        break;

    }
    button2_debounce_time = millis();

  }
}

void left_button_isr(){

  if(millis()-button1_debounce_time > DEBOUNCE_TIME){
        Serial.println("left");

  switch(oled.get_selection()){
    case P_GAIN_SELECT:
      can.set_p_gain(state.p - can.increments.p);
      break;
    case D_GAIN_SELECT:
      can.set_d_gain(state.d - can.increments.d);
      break;
    case LOW_TARGET:
      can.set_low_target(state.low_target - can.increments.low_target);
      break;
    case HIGH_TARGET:
      can.set_high_target(state.high_target - can.increments.high_target);
      break;
  }
  button1_debounce_time = millis();
  }
}

void bottom_button_isr(){
  if(millis()- button3_debounce_time > DEBOUNCE_TIME){
      Serial.println("bottom");

    oled.bottom_button_isr();
    button3_debounce_time = millis();
  }
}


void mailbox_callback(const CAN_message_t& msg){
  can.parse_message(msg);
}


bool LED_STATUS = HIGH;
//
void setup(void) {
  Serial.begin(9600);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  pinMode(BUTTON5, INPUT_PULLUP);

  attachInterrupt(BUTTON1, left_button_isr, FALLING);
  attachInterrupt(BUTTON2, right_button_isr, FALLING);
  attachInterrupt(BUTTON3, bottom_button_isr, FALLING);
 
  oled = OLED(&state);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LED_STATUS);
  Serial.println("Finished Pinmodes");

  flexcan_bus.begin();
  flexcan_bus.setBaudRate(FLEXCAN_BAUD_RATE);
  flexcan_bus.setMaxMB(FLEXCAN_MAX_MAILBOX);
  flexcan_bus.enableFIFO();
  flexcan_bus.enableFIFOInterrupt();
  flexcan_bus.onReceive(mailbox_callback);
  oled.idle();
}

void loop(void) {
  if(millis() - can.last_CAN_msg < 500){
    can.send_controller_toggle(!digitalRead(BUTTON4));
    can.send_clutch(!digitalRead(BUTTON5));
    oled.graph_eg_rpm();
    oled.draw_menu();
  }
  else oled.idle();
}