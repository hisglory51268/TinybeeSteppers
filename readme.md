= Stepper Library for use with mkstinybee =

Library for managing stepper motors connected to a MKS tinybee
because all I/Os 128-151 are sent through the shift registers 74HC595 it was easier create a library handle the steppers and the I/Os 144-151 as a standard library like acellstepper would not work.

The I/Os are outputs only. Designed for the MKS tinybee however the library could be modified to work with other boards using shift registers as well
also the motors have been renamed to 1-5
access to IO144-IO151 using the coomands getIO() and setIO() IO144-IO151 have been remapped to 16-23 the actuall number to the shift registers

= Stepper Library for use with mkstinybee =

Library for managing stepper motors connected to a MKS tinybee
because all I/Os 128-151 are sent through the shift registers 74HC595 it was easier create a library handle the steppers and the I/Os 144-151 as a standard library like acellstepper would not work.

The I/Os are outputs only. Designed for the MKS tinybee however the library could be modified to work with other boards using shift registers as well
also the motors have been renamed to 1-5
access to IO144-IO151 using the coomands getIO() and setIO() IO144-IO151 have been remapped to 16-23 the actuall number to the shift registers


**Example**

#include <tinybeeSteppers.h>

unsigned long fwdtimeout = 0;
unsigned long bwdtimeout = 0;

tinybeeSteppers TBSR = tinybeeSteppers();

void setup() {
  // put your setup code here, to run once:
  Serial.print("TBSR Status: ");
  TBSR.begin(27,25,26);
  
  TBSR.set_powersave(0,1);//sets powersave option on motor 1 defaults to off after 1 second
  TBSR.set_speed(48,1);// speed of motor more specifically delay in ms between motor stepps between o and 50: (50, no delay) (0, 50ms delay)
  TBSR.set_default_direction(1, 1);//(direction 0 or 1 reverses default direction of motor 1)
  
  TBSR.set_powersave(0,2);
  TBSR.set_speed(48,2);
  TBSR.set_default_direction(1, 2);
  
  TBSR.set_powersave(0,3);
  TBSR.set_speed(48,3);
  TBSR.set_default_direction("CCW", 3);
  
  TBSR.set_powersave(0,2);
  TBSR.set_speed(48,2);
  TBSR.set_default_direction("CCW", 4);
  
  TBSR.set_powersave(0,5);
  TBSR.set_speed(48,5);
  TBSR.set_default_direction("CCW", 5);


fwdtimeout = millis() + 5000;
bwdtimeout = millis() + 10000;

    //the next 14 lines will make the ios 144-148 blink
    TBSR.setIO(16,0);  
    TBSR.setIO(17,0);  
    TBSR.setIO(18,0);  
    TBSR.setIO(19,0);  
    TBSR.setIO(20,0);  
    TBSR.handle_TBS_Steppers();
    delay(500);
    TBSR.setIO(16,1);  
    TBSR.setIO(17,1);  
    TBSR.setIO(18,1);  
    TBSR.setIO(19,1);  
    TBSR.setIO(20,1);  
    TBSR.handle_TBS_Steppers();
    delay(500);

}

void loop() {
if (checkTime(fwdtimeout)){
  TBSR.Step(500,1);//tells motor 1 to move 500 stepps
  TBSR.Step(500,2);
  TBSR.Step(500,3);
  TBSR.Step(500,4);
  TBSR.Step(500,5);
  fwdtimeout = millis() + 10000;
  }
  
if (checkTime(bwdtimeout)){
//  int o_step = -500;
  TBSR.Step((500-1000),1);//tells motor 1 to move -500 stepps
  TBSR.Step((500-1000),2);
  TBSR.Step((500-1000),3);
  TBSR.Step((500-1000),4);
  TBSR.Step((500-1000),5);
  bwdtimeout = millis() + 10000;
  }
  
  TBSR.handle_TBS_Steppers();// actually moves the motors
}

bool checkTime(unsigned long m_time){
  if (millis() >= m_time){
      return true;
    }else{
      return false;
    }
}
