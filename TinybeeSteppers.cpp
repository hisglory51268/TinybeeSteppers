/*  tinybeeSteppers.CPP -library for managing stepper motors connected to a MKS tinybee
 *   because all I/Os 128-151 are sent through the shift registers: It was easier to build a library to handle the steppers and the I/Os 144-151. 
 *   the I/Os are outputs only. Designed for the MKS tinybee however the library could be modified to work with other boards using shift registers as well 
 *   also the motors have been renamed to 1-5
 *   Created by Jeffrey Rydell 7,13,2024
 *   Released into the public domain.
*/

#include "TinybeeSteppers.h"
#include "Arduino.h"
#include <ShiftRegister74HC595.h>


#define X_EN 0
#define X_STEP 1
#define X_DIR 2
#define Y_EN 3
#define Y_STEP 4
#define Y_DIR 5
#define Z_EN 6
#define Z_STEP 7

#define Z_DIR 8
#define E0_EN 9
#define E0_STEP 10
#define E0_DIR 11
#define E1_EN 12
#define E1_STEP 13
#define E1_DIR 14

#define H_BED 16
#define H_E0 17
#define H_E1 18
#define H_FAN1 19
#define FAN2 20
#define BEEPER 21
#define OUT_150 22
#define OUT_151 23

//ShiftRegister74HC595<3> TBS(27, 25, 26);



TinybeeSteppers::TinybeeSteppers(){
//  int serialDataPin = 27; // DS
//  int clockPin = 25; // SHCP
//  int latchPin = 26; // STCP
//  ShiftRegister74HC595<3> TBS(serialDataPin, clockPin, latchPin);
//  _TBS(serialDataPin, clockPin, latchPin);

//  _TBS = nullptr;

  _pinchange = false;
  
  _pinvalues[0] = B11111111;
  _pinvalues[1] = B11111111;
  _pinvalues[2] = 0;//B00000000
  
//  _pinvalues[2] = B11111111;

  _xtimeout = 0; // time in ms to perform the next step
  _ytimeout = 0;
  _ztimeout = 0;
  _e0timeout = 0;
  _e1timeout = 0;

  _x_steps = 0; //how many steps to turn the motor
  _y_steps = 0;
  _z_steps = 0;
  _e0_steps = 0;
  _e1_steps = 0;
  
  _x_speed = 50;// speed of the motor, inverse delay 0 adds a 500ms delay 500 adds a 0 ms delay
  _y_speed = 50;
  _z_speed = 50;
  _e0_speed = 50;
  _e1_speed = 50;
  
  _x_powersave = 1; // 1 if powersave is on
  _y_powersave = 1;
  _z_powersave = 1;
  _e0_powersave = 1;
  _e1_powersave = 1;

  
  _XD_DIR = 0;
  _YD_DIR = 0;
  _ZD_DIR = 0;
  _E0D_DIR = 0;
  _E1D_DIR = 0;
   
  }
void TinybeeSteppers::begin(uint8_t serialDataPin,uint8_t clockPin,uint8_t latchPin){

//  if (TBS) {
    delete _TBS; // remove old interface
//  }

//  i2c_dev = new Adafruit_I2CDevice(BMP085_I2CADDR, wire); 
  _TBS = new ShiftRegister74HC595<3>(serialDataPin, clockPin, latchPin);
//  _TBS = TBS;

//  if (!TBS->begin()) {
//    return false;
//  }

//  return true;
 
}

void TinybeeSteppers::setTbsPinState(uint8_t Q_pin, uint8_t state){
  /* sets the pin output value of a single pin on the shiftregister */
    _pinchange = true;
//    _pinvalues[0] &= (state << Q_pin);
//
    int out_shifter = 0;
    int out_pin = 0;
    if (Q_pin <= 7){
       out_shifter = 0;
       out_pin = Q_pin;
    }
    else if ((Q_pin >= 8) && (Q_pin <= 15)){
      out_shifter = 1;
      out_pin = Q_pin - 8;
      }
    else {//if ((Q_pin >= 16) && (Q_pin <= 23))
      out_shifter = 2;
      out_pin = Q_pin - 16;
    }
    bitWrite(_pinvalues[out_shifter], out_pin, state);

}

bool TinybeeSteppers::checkTime(unsigned long m_time){
  if (millis() >= m_time){
      return true;
    }else{
      return false;
    }
}

bool TinybeeSteppers::toggleValue(bool value){
  switch (value){
    case false:
      return true;
      break;
    case true:
      return false;
      break;
  }
}

void TinybeeSteppers::Step(int steps,uint8_t motor){
  /* moves the selected moter x, the motors are mapped to x=1,y=2,z=3,e0=4,e1=5*/
  switch (motor){
    case 1://"X"
      setTbsPinState(X_EN,0);
      _x_steps = steps;
      break; 
      
    case 2://"Y"
      setTbsPinState(Y_EN,0);
      _y_steps = steps;
      break; 
      
    case 3://"Z"
      setTbsPinState(Z_EN,0);
      _z_steps = steps;
      break; 
      
    case 4://"E0"
      setTbsPinState(E0_EN,0);
      _e0_steps = steps;
      break; 
      
    case 5://"E1"
      setTbsPinState(E1_EN,0);
      _e1_steps = steps;
      break; 
  }
}

void TinybeeSteppers::set_powersave(uint8_t value,uint8_t motor){
  /* moves the selected moter x, the motors are mapped to x=1,y=2,z=3,e0=4,e1=5*/
  switch (motor){
    case 1://"X"
      _x_powersave = value;
      break; 
      
    case 2://"Y"
      _y_powersave = value;
      break; 
      
    case 3://"Z"
      _z_powersave = value;
      break; 
      
    case 4://"E0"
      _e0_powersave = value;
      break; 
      
    case 5://"E1"
      _e1_powersave = value;
      break; 
  }
}

void TinybeeSteppers::set_speed(int Speed,uint8_t motor){
  /* moves the selected moter x, the motors are mapped to x=1,y=2,z=3,e0=4,e1=5*/
  switch (motor){
    case 1://"X"
      _x_speed = Speed;
      break; 
      
    case 2://"Y"
      _y_speed = Speed;
      break; 
      
    case 3://"Z"
      _z_speed = Speed;
      break; 
      
    case 4://"E0"
      _e0_speed = Speed;
      break; 
      
    case 5://"E1"
      _e1_speed = Speed;
      break; 
  }
}

void TinybeeSteppers::set_default_direction(bool d_dir,uint8_t motor){
  /* moves the selected moter x, the motors are mapped to x=1,y=2,z=3,e0=4,e1=5*/
  switch (motor){
    case 1://"X"
      _XD_DIR = d_dir;
      break; 
      
    case 2://"Y"
      _YD_DIR = d_dir;
      break; 
      
    case 3://"Z"
      _ZD_DIR = d_dir;
      break; 
      
    case 4://"E0"
      _E0D_DIR = d_dir;
      break; 
      
    case 5://"E1"
      _E1D_DIR = d_dir;
      break; 
  }
}


void TinybeeSteppers::setIO(uint8_t IO, uint8_t value){
//  144 = 16
//  uint8_t pinNum = IO - 128;
setTbsPinState(IO,value);
}

uint8_t TinybeeSteppers::getIO(uint8_t IO){
//  144 = 16
//  uint8_t pinNum = IO - 128;
 return _TBS->get(IO);
 
}

String TinybeeSteppers::debugPinvalues(){
return "[0]"+ String(_pinvalues[0])+" [1] "+ String(_pinvalues[1])+" [2] "+ String(_pinvalues[2]);
}

void TinybeeSteppers::cycleTBS(){
    if (_pinchange == true){//has pin value changed
      _TBS->setAll(_pinvalues);
      setTbsPinState(X_STEP,1);
      setTbsPinState(Y_STEP,1);
      setTbsPinState(Z_STEP,1);
      setTbsPinState(E0_STEP,1);
      setTbsPinState(E1_STEP,1);
      _TBS->setAll(_pinvalues);
      _pinchange = false;
    }
}


void TinybeeSteppers::handle_TBS_Steppers(){
  uint8_t dir = 0;
  int steps = 0;

  if (checkTime(_xtimeout)){
    if (_x_steps != 0){
      dir = int_to_dir(_x_steps);
      _x_steps = returnNewStep(_x_steps, dir);
      set_stepper_step(checkDefaultDir(dir,_XD_DIR),X_STEP,X_DIR);
      _xtimeout = return_stepper_timeout(_x_speed);
    } 
    checkStepperPowersave(_xtimeout,X_EN,_x_powersave);
  }
  
  if (checkTime(_ytimeout)){
    if (_y_steps != 0){
      dir = int_to_dir(_y_steps);
      _y_steps = returnNewStep(_y_steps, dir);
      set_stepper_step(checkDefaultDir(dir,_YD_DIR),Y_STEP,Y_DIR);
      _ytimeout = return_stepper_timeout(_y_speed);
    } 
    checkStepperPowersave(_ytimeout,Y_EN,_y_powersave);
  }
  
  if (checkTime(_ztimeout)){
    if (_z_steps != 0){
      dir = int_to_dir(_z_steps);
      _z_steps = returnNewStep(_z_steps, dir);
      set_stepper_step(checkDefaultDir(dir,_ZD_DIR),Z_STEP,Z_DIR);
      _ztimeout = return_stepper_timeout(_z_speed);
    } 
    checkStepperPowersave(_ztimeout,Z_EN,_z_powersave);
  }
  
  if (checkTime(_e0timeout)){
    if (_e0_steps != 0){
      dir = int_to_dir(_e0_steps);
      _e0_steps = returnNewStep(_e0_steps, dir);
      set_stepper_step(checkDefaultDir(dir,_E0D_DIR),E0_STEP,E0_DIR);
      _e0timeout = return_stepper_timeout(_e0_speed);
    } 
    checkStepperPowersave(_e0timeout,X_EN,_e0_powersave);
  }
  
  if (checkTime(_e1timeout)){
    if (_e1_steps != 0){
      dir = int_to_dir(_e1_steps);
      _e1_steps = returnNewStep(_e1_steps, dir);
      set_stepper_step(checkDefaultDir(dir,_E1D_DIR),E1_STEP,E1_DIR);
      _e1timeout = return_stepper_timeout(_e1_speed);
    } 
    checkStepperPowersave(_e1timeout,E1_EN,_e1_powersave);
  }
  cycleTBS();
}


void TinybeeSteppers::checkStepperPowersave(unsigned long timeout, byte enable_pin, uint8_t powersave){
  /* handles the powersave for one stepper*/
  if (powersave == 1){
    if ((millis() >= (timeout + 1000)) && (millis() <= (timeout + 2000)) ){
      if (_TBS->get(enable_pin) == 0){
        setTbsPinState(enable_pin,1);
      }
    }
  }
}

void TinybeeSteppers::set_stepper_step(int dir, uint8_t step_pin, uint8_t dir_pin){
  /* sets a single stepper step */
  setTbsPinState(dir_pin,dir);
  setTbsPinState(step_pin,0);
}


uint8_t TinybeeSteppers::int_to_dir(int in_int){
  /* converts int to motor direction and reverses direction baised on default direction*/
  uint8_t out_dir = 0;
  if (in_int <= 0){
    return 0 ;
  }else{
    return 1;
  }
}

uint8_t TinybeeSteppers::checkDefaultDir(uint8_t in_dir, uint8_t d_dir){
  if (d_dir == 1){
    return toggleValue(in_dir);
  }else{
    return in_dir;
  }
}
 
int TinybeeSteppers::returnNewStep(int in_int, uint8_t dir){
  /*de increments the step */
  if (dir == 1){
    return in_int - 1;
  }
  if (dir == 0){
    return in_int + 1;
  }
 
}

unsigned long TinybeeSteppers::return_stepper_timeout(int in_timeout){
  /*gets delay from positive speed i.e. 50 = 0 or 0 = 50 */
  int out_timeout = map(in_timeout,0,50, 50, 0);
  return  millis() + out_timeout;
  
}
