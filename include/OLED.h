#ifndef OLED_H
#define OLED_H
#include <Arduino.h>
#include <U8g2lib.h>
#include<string>
#include <circularBuffer.h>
#include <StateVars.h>

//Pins for Top OLED Screen
#define OLED1_SA0    20  
#define OLED1_RESET  21                   
#define OLED1_SDA_IN 18    
#define OLED1_CLK    19               
#define OLED1_BS1        40 
#define OLED1_BS2        41 

//Pins for Bottom OLED Screen
#define OLED2_SA0    15  
#define OLED2_RESET  37                   
#define OLED2_SDA_IN 17    
#define OLED2_CLK    16               
#define OLED2_BS1        38 
#define OLED2_BS2        39 


#define OLED_CLK_SPEED 1000000UL
#define OLED_SlaveAddress 0x3D
#define MAX_RPM  4000.0
#define GRAPH_NUMPIXELS_X  64.0
#define GRAPH_NUMPIXELS_Y 35.0
#define ybin_size (MAX_RPM/GRAPH_NUMPIXELS_Y)

//codes for the menu selection
#define    P_GAIN_SELECT 0
#define    D_GAIN_SELECT 1
#define    LOW_TARGET 2
#define    HIGH_TARGET 3

class OLED{
    public:
        OLED(StateVars* state);
        OLED();
        void init();
        void graph_eg_rpm();
        void draw_menu();

        void bottom_button_isr();
        int get_selection();
        void idle();

        circularBuffer graph_frame;
        circularBuffer target_rpms;

    private:

        StateVars* state;
        U8G2_SSD1305_128X32_NONAME_F_HW_I2C     oled1 = U8G2_SSD1305_128X32_NONAME_F_HW_I2C(U8G2_R0, OLED1_RESET);
        U8G2_SSD1305_128X32_NONAME_F_2ND_HW_I2C oled2 = U8G2_SSD1305_128X32_NONAME_F_2ND_HW_I2C(U8G2_R0, OLED2_RESET);
        int selection = P_GAIN_SELECT;
};



#endif