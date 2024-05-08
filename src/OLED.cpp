#include <OLED.h>

OLED::OLED(StateVars* state_in){
  Serial.println("OLED constructor");
  state = state_in;
  
  init();
}
OLED::OLED(){
  Serial.println("OLED constructor");

}

void OLED::init(){

// OLED1 Pinmodes
  pinMode(OLED1_RESET, OUTPUT);
  pinMode(OLED1_BS2, OUTPUT);
  pinMode(OLED1_BS1, OUTPUT);
  pinMode(OLED1_SA0, OUTPUT);
  pinMode(OLED1_SDA_IN, OUTPUT);  

// OLED2 Pinmodes
  pinMode(OLED2_RESET, OUTPUT);
  pinMode(OLED2_BS2, OUTPUT);
  pinMode(OLED2_BS1, OUTPUT);
  pinMode(OLED2_SA0, OUTPUT);
  pinMode(OLED2_SDA_IN, OUTPUT); 

  //OLED1 SA and interface select
  digitalWrite(OLED1_SA0, HIGH); 
  digitalWrite(OLED1_BS1, HIGH);
  digitalWrite(OLED1_BS2, LOW);
  //OLED1 SA and interface select

  digitalWrite(OLED2_SA0, HIGH); 
  digitalWrite(OLED2_BS1, HIGH);
  digitalWrite(OLED2_BS2, LOW);

//OLED1 Reset activation routine
  digitalWrite(OLED1_RESET, LOW);
  delay(150);
  digitalWrite(OLED1_RESET, HIGH);
  delay(150);

//OLED2 Reset activation routine
  digitalWrite(OLED2_RESET, LOW);
  delay(150);
  digitalWrite(OLED2_RESET, HIGH);
  delay(150);

// OLED1 u8g2 settings
  oled1.setI2CAddress(OLED_SlaveAddress * 2);
  oled1.begin();
  oled1.setBusClock(OLED_CLK_SPEED);
  oled1.clearBuffer();
  oled1.setColorIndex(1);
  oled1.setDrawColor(1);
  oled1.setFont(u8g2_font_crox1h_tf);


// OLED2 u8g2 settings
  oled2.setI2CAddress(OLED_SlaveAddress * 2);
  oled2.begin();
  oled2.setBusClock(OLED_CLK_SPEED);
  oled2.clearBuffer();
  oled2.setColorIndex(1);
  oled2.setDrawColor(1); 
  oled2.setFont(u8g2_font_crox1h_tf);

}

void OLED::graph_eg_rpm(){
  oled1.clearBuffer();
  oled1.drawFrame(2,0,GRAPH_NUMPIXELS_X  ,GRAPH_NUMPIXELS_Y - 3);
  oled1.drawStr(GRAPH_NUMPIXELS_X+5, 12, "EG:");
  oled1.drawStr(GRAPH_NUMPIXELS_X+5, 30, "WL:");
  oled1.setCursor(GRAPH_NUMPIXELS_X+25, 12);
  oled1.print(state->eg_rpm,0); 
  
  oled1.setCursor(GRAPH_NUMPIXELS_X+25, 30);
  oled1.print(state->wl_rpm,1);

  for(int i = 0; i < GRAPH_NUMPIXELS_X; i++){
    oled1.drawPixel(i+2, GRAPH_NUMPIXELS_Y-(graph_frame.getValAtIndex(GRAPH_NUMPIXELS_X-i) / 25)-4); //replace dataset[] with circular buffer
  }
  oled1.sendBuffer();
}

void OLED::draw_menu(){
    //  oled1.drawStr(GRAPH_NUMPIXELS_X+5, 30, "WL:");
    //TODO: 4X prints, 1x draw frame around current selection
    
  oled2.clearBuffer();
  oled2.setCursor(4,11);
  oled2.print("P: ");
  oled2.print(state->p,3);

  oled2.setCursor(4,25);
  oled2.print("D: ");
  oled2.print(state->d,3);

  oled2.setCursor(50,11);
  oled2.print("LO_TARG: ");
  oled2.print(state->low_target,0);

  oled2.setCursor(50,25);
  oled2.print("HI_TARG: ");
  oled2.print(state->high_target,0);

  switch(selection){
    case P_GAIN_SELECT:
      oled2.drawFrame(2,0,45,13);
      break;
    case D_GAIN_SELECT:
      oled2.drawFrame(2,14,45,13);
      break;
    case LOW_TARGET:
      oled2.drawFrame(48,0,75,13);
      break;
    case HIGH_TARGET:
      oled2.drawFrame(48,14,75,13);
      break;
  }

  oled2.drawBox(2,30,state->act_pos*(128/15),2);
  oled2.sendBuffer();
    //TODO: Bar for Actuator position
}

void OLED::bottom_button_isr(){
  //Serial.println("tripped");

  if(selection != HIGH_TARGET) {
    selection++;
    return;
    }
  else if(selection == HIGH_TARGET) selection = P_GAIN_SELECT;
};
int OLED::get_selection(){
  return selection;
}
void OLED::idle(){
    oled1.clearBuffer();
    oled2.clearBuffer();
    oled1.setFont(u8g2_font_inb16_mr);
    oled2.setFont(u8g2_font_inb16_mr);

    oled1.setCursor(2,24);
    oled1.print("GOOD LUCK");

    oled2.setCursor(2,24);
    oled2.print("DONT CRASH");

    oled1.sendBuffer();
    oled2.sendBuffer();
}