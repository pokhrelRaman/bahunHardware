
#include "UnoJoy.h"

  #define inputCLK 4
  #define inputDT 5

  int prevState[2], currentState[2];
  
  /* State A == 00;
   *  State B == 01;
   *  State C == 10;
   *  State D == 11;
   */

  
  int counter = 90,pinA , pinB;
  int valueX;

 void setupEncoder() 
{  
   pinMode (inputCLK,INPUT);
   pinMode (inputDT,INPUT);
   pinMode(A5, INPUT);
  digitalWrite(A5, HIGH);
   
} 

void setup(){
  setupPins();
  setupUnoJoy();
  
}

void loop(){
  // Always be getting fresh data
  dataForController_t controllerData = getControllerData();
  setControllerData(controllerData);
}

void setupPins(void){
  // Set all the digital pins as inputs
  // with the pull-up enabled, except for the 
  // two serial line pins
  for (int i = 2; i <= 12; i++)
  {
    if(i != 4 || i != 5)
    {
      pinMode(i, INPUT);
      digitalWrite(i, HIGH);
    }
  }
  pinMode(A4, INPUT);
  digitalWrite(A4, HIGH);
  pinMode(A5, INPUT);
  digitalWrite(A5, HIGH);
}

bool encoderValue() 
{ 
   pinA = !digitalRead(inputCLK);
   pinB = !digitalRead(inputDT);
   currentState[0] = pinA;
   currentState[1] = pinB;

   for(int i = 0 ; i<2  ; i++)
   {
      if(prevState[i] != currentState[i])
      {
        prevState[0] = currentState[0];
        prevState[1] = currentState[1];
        counter++;
                  if(counter >255)
                 {
                  counter = 0;
                 }
                 if(counter <0)
                 {
                  counter = 255;
                 }
      }
   }
    if(counter <= 180)
    { 
     return true;
    }

   else return false;

   
  
  }
dataForController_t getControllerData(void){
  
  // Set up a place for our controller data
  //  Use the getBlankDataForController() function, since
  //  just declaring a fresh dataForController_t tends
  //  to get you one filled with junk from other, random
  //  values that were in those memory locations before
  dataForController_t controllerData = getBlankDataForController();
  // Since our buttons are all held high and
  //  pulled low when pressed, we use the "!"
  //  operator to invert the readings from the pins
  controllerData.triangleOn = !digitalRead(2);
  controllerData.circleOn = !digitalRead(3);
 // controllerData.squareOn = !digitalRead(4);
  //controllerData.crossOn = !digitalRead(5);
  if(counter < 180)
  {
    controllerData.squareOn = !encoderValue();
  }
  controllerData.dpadUpOn = !digitalRead(6);
  controllerData.dpadDownOn = !digitalRead(7);
  controllerData.dpadLeftOn = !digitalRead(8);
  controllerData.dpadRightOn = !digitalRead(9);
  controllerData.l1On = !digitalRead(10);
  controllerData.r1On = encoderValue();
  controllerData.selectOn = !digitalRead(12);
  controllerData.startOn = !digitalRead(A4);
  controllerData.homeOn = !digitalRead(A5);
  
  // Set the analog sticks
  //  Since analogRead(pin) returns a 10 bit value,
  //  we need to perform a bit shift operation to
  //  lose the 2 least significant bits and get an
  //  8 bit number that we can use  
  controllerData.leftStickX = counter;

  // And return the data!
  return controllerData;
}
