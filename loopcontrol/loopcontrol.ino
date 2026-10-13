// Radu Corcodel (c) 2025, Brookline MA-02445

// Libraries and other deps
#include <AccelStepper.h>

// Pinout Config
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define sleepPin 4
#define resetPin 5
#define M0Pin 8
#define M1Pin 7
#define M2Pin 6
#define enablePin 9
#define switchDirectionA 10
#define switchDirectionB 11
#define switchSpeed 12

// Globals
AccelStepper stepper(motorInterfaceType, stepPin, dirPin); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
int swA_val = 0; //keeps track of the direction we turn the capacitor
int swB_val = 0;
int swSpeed_val = 0;
bool highSpeed = false;

void setHighSpeed(void){
   if (!highSpeed) { //Avoid rewriting the pins that are already set to the correct microstep
      digitalWrite(M0Pin, LOW);
      digitalWrite(M1Pin, LOW);
      digitalWrite(M2Pin, LOW);
      highSpeed = true;
   }
}

void setLowSpeed(void){
   if (highSpeed) {
      digitalWrite(M0Pin, HIGH);
      digitalWrite(M1Pin, HIGH);
      digitalWrite(M2Pin, HIGH);
      highSpeed = false;
   }
}

void setup()
{  
   // Some serial output for diagnostics
   Serial.begin(57600);
   //Set the Ardiono pin directions
   pinMode(dirPin, OUTPUT); 
   pinMode(stepPin, OUTPUT); 
   pinMode(sleepPin, OUTPUT);
   pinMode(resetPin, OUTPUT);
   pinMode(enablePin, OUTPUT);
   pinMode(M0Pin, OUTPUT);
   pinMode(M1Pin, OUTPUT);
   pinMode(M2Pin, OUTPUT);
   pinMode(switchSpeed, INPUT);
   pinMode(switchDirectionA, INPUT);
   pinMode(switchDirectionA, INPUT);
   pinMode(LED_BUILTIN, OUTPUT);

   //Enable the motor driver
   digitalWrite(sleepPin, HIGH);
   digitalWrite(resetPin, HIGH);
   digitalWrite(enablePin, LOW); // Enabled=LOW
   Serial.println("Motor driver enabled!");

   //Set the microstep configuration
   setHighSpeed();
   //setLowSpeed();

   // Example constant speed
   stepper.setMaxSpeed(10000); // Determined experimentally
   // stepper.setSpeed(500);	//1000
   stepper.setAcceleration(170); // Determined experimentally
   // stepper.setCurrentPosition(0); //Wherever the shaft is now is considered 0 deg
}

void loop()
{  
   //Example constant speed
   // stepper.runSpeed();

   // // Go to position with accel/decel. This is blocking so don't use with event-driven loops
   // stepper.runToNewPosition(5000);
   // delay(300);
   // stepper.runToNewPosition(1000);
   // delay(300);
   // stepper.runToNewPosition(12000);
   // delay(300);

   swA_val = digitalRead(switchDirectionA);
   swB_val = digitalRead(switchDirectionB);
   swSpeed_val = digitalRead(switchSpeed);

   if (swSpeed_val == 0) {
      setHighSpeed();
   }
   else {
      setLowSpeed();
   }

   if (swA_val == 0 ||  swB_val == 0) { //|| swSpeed_val == 0
      digitalWrite(LED_BUILTIN, HIGH); //Visual check for code and HW functionality
      if (swA_val == 0) {
         stepper.setSpeed(500);
      }
      if (swB_val == 0) {
         stepper.setSpeed(-500);
      }
      stepper.runSpeed();
   }
   else {
      digitalWrite(LED_BUILTIN, LOW);
      stepper.stop();
   }
   


}
