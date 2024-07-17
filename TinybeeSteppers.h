/*  TinybeeSteppers.h -library for managing stepper motors connected to a MKS tinybee
 *   because all I/Os 128-151 are sent through the shift registers it was easier to library handle the steppers and the I/Os 144-151 
 *   the I/Os are outputs only. Designed for the MKS tinybee however the library could be modified to work with other boards using shift registers as well 
 *   also the motors have been renamed to 1-5
 *   Created by Jeffrey Rydell 7,13,2024
 *   Released into the public domain.
*/
#ifndef TinybeeSteppers_h
#define TinybeeSteppers_h
#include "Arduino.h" 
#include <ShiftRegister74HC595.h>

class TinybeeSteppers 
{
public:
        TinybeeSteppers();
        void begin(uint8_t serialDataPin,uint8_t clockPin,uint8_t latchPin);
        void Step(int steps, uint8_t motor);
        
        void set_powersave(uint8_t value, uint8_t motor);
        void set_speed(int Speed, uint8_t motor);
        void set_default_direction(bool d_dir, uint8_t motor);
        
        void setIO(uint8_t IO, uint8_t value);
        uint8_t getIO(uint8_t IO);
        void handle_TBS_Steppers(void);
        String debugPinvalues(void);        

private:
void setTbsPinState(uint8_t Q_pin, uint8_t state);
bool checkTime(unsigned long m_time);
bool toggleValue(bool value);
void cycleTBS();
void checkStepperPowersave(unsigned long timeout, byte enable_pin, uint8_t powersave);
void set_stepper_step(int dir, uint8_t step_pin, uint8_t dir_pin);
uint8_t int_to_dir(int in_int);
int returnNewStep(int in_int, uint8_t dir);
uint8_t checkDefaultDir(uint8_t out_dir, uint8_t d_dir);

unsigned long return_stepper_timeout(int in_timeout);
//ShiftRegister74HC595<3> *TBS;
ShiftRegister74HC595<3> *_TBS;
//ShiftRegister74HC595<3> * _TBS(27, 25, 26);

// all the values sent to the shift registers
uint8_t _pinvalues[3] = {0,0,0};


//uint8_t pinvalues[2] = {B11111111,B11111111,B00000000};
//unsigned byte pinvalues[2]={255,255,0};


bool _pinchange;


//uint8_t pinValues[] = { };



unsigned long _xtimeout; // time in ms to perform the next step
unsigned long _ytimeout;
unsigned long _ztimeout;
unsigned long _e0timeout;
unsigned long _e1timeout;

int _x_steps; //how many steps to turn the motor
int _y_steps;
int _z_steps;
int _e0_steps;
int _e1_steps;

int _x_speed;// speed of the motor, inverse delay 0 adds a 500ms delay 500 adds a 0 ms delay
int _y_speed;
int _z_speed;
int _e0_speed;
int _e1_speed;

uint8_t _x_powersave; // 1 if powersave is on
uint8_t _y_powersave;
uint8_t _z_powersave;
uint8_t _e0_powersave;
uint8_t _e1_powersave;


// default direction of the motor change to CCW to spin counter clock wise
uint8_t _XD_DIR;
uint8_t _YD_DIR;
uint8_t _ZD_DIR;
uint8_t _E0D_DIR;
uint8_t _E1D_DIR;

//unsigned long hbedtimeout = 0;
//unsigned long he0timeout = 0;
//unsigned long he1timeout = 0;
//unsigned long fan1timeout = 0;
//unsigned long fan2timeout = 0;
//unsigned long beepertimeout = 0;

//unsigned long fwdtimeout = 0;
//unsigned long bwdtimeout = 0;

//uint8_t hbed = 0;
//uint8_t he0 = 0;
//uint8_t he1 = 0;
//uint8_t fan1 = 0;
//uint8_t fan2 = 0;
//uint8_t beeper = 0;
//uint8_t pin150 = 0;
//uint8_t pin151 = 0;

};
#endif
