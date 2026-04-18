//XXXXXXXXXXXXXXXXXXXXXXX//
//VERSION 1.0
//BY ORGLLK
//XXXXXXXXXXXXXXXXXXXXXXX//
#include <Arduino.h>
#include"DHT.h"
// Ultrasonic sensor pins////////////////////
#define ultt1       22    // Trigger pin for sensor 1
#define ultr1       23    // Echo pin for sensor 1
#define ultt2       24    // Trigger pin for sensor 2
#define ultr2       25    // Echo pin for sensor 2
//////////////////////////////////////////
// Relay and indicators// //
#define relay       10    //Relay pin for controlling the motor
#define relaystats  11    // ``Relay status led for indicating motor state
/////Main SW&LED/////     //
#define mainswitch  8     //Main switch pin for turning on/off the system
#define mainled     9     //Main power led indicator
//////////////////        // 
//warning system//        //
#define senserled   5     //sensor issue led for warning system
#define falutled    7     //Lerd for fault detection
#define elecprob    27    // Electric problem pin for fault detection
#define reset       52    // DEV
#define DHTPIN      26    // GPIO pin for DHT11 data
DHT dht;            //    // Create DHT object
#define overheatp   6     // Overheat cut off protection
void setup() {
  Serial.begin(9600);

  // Ultrasonic pins
  pinMode(ultt1,      OUTPUT);
  pinMode(ultr1,      INPUT);
  pinMode(ultt2,      OUTPUT);
  pinMode(ultr2,      INPUT);
                      
  // Relay and indicators
  pinMode(relay,      OUTPUT);
  pinMode(relaystats, OUTPUT);
  pinMode(mainled,    OUTPUT);
  pinMode(mainswitch, INPUT);

  ///////warning system//////
  pinMode(overheatp,  OUTPUT);
  pinMode(falutled,   OUTPUT);
  pinMode(senserled,  OUTPUT);
  pinMode(elecprob,   INPUT);
  pinMode(reset,      INPUT); // Reset button with pull-up resistor
  dht.setup(DHTPIN);  // Initialize sensor on defined pin
}

void loop() {
  ///////////////////////////TEMP/////////////////////////////////////////////////
  delay(dht.getMinimumSamplingPeriod());  // Respect sensor timing
  float temperatureC = dht.getTemperature();   // Read temperature in Celsius
  /////                           x                                  ////////////
  if(temperatureC >=70 ){
    digitalWrite(overheatp,LOW);
  }else{
    digitalWrite(overheatp,HIGH);
  }
  ///////////////////////////TEMP////////////////////////////////////////////////
  ////////                   X                                       ///////////
  if(digitalRead(reset) == HIGH) { // Check if reset button is pressed
    digitalWrite(senserled, LOW);    // Sensor issue LED OFF
    digitalWrite(falutled, LOW);     // Fault LED OFFe
  }
  // Trigger sensor 1
  digitalWrite(ultt1, LOW);
  delayMicroseconds(2);
  digitalWrite(ultt1, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultt1, LOW);
  digitalWrite(mainled, HIGH); // Indicate system is active

  // Read echo from sensor 1
  long duration1 = pulseIn(ultr1, HIGH);
  int distance1 = duration1 * 0.034 / 2;
  Serial.print("Distance1: ");
  Serial.print(distance1);
  Serial.print("    TEMP: ");
  Serial.println(temperatureC, 1);


  // Trigger sensor 2
  digitalWrite(ultt2, LOW);
  delayMicroseconds(2);
  digitalWrite(ultt2, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultt2, LOW);

  // Read echo from sensor 2
  long duration2 = pulseIn(ultr2, HIGH);
  int distance2 = duration2 * 0.034 / 2;
  Serial.print("Distance2: ");
  Serial.print(distance2);
  Serial.print("     TEMP: ");
  Serial.println(temperatureC, 1);

if(digitalRead(mainswitch) == HIGH){
  if(digitalRead(elecprob) == HIGH) {
    digitalWrite(relay, LOW);        // Motor OFF
    Serial.println("Fault detected, ");
    digitalWrite(relaystats, LOW);
    digitalWrite(falutled, HIGH); // Warning ON
  }else {
      if(distance1 < 18 || distance1 > 81 || distance2 < 18 || distance2 > 81){
       digitalWrite(senserled, HIGH); // Warning ON
       digitalWrite(falutled, HIGH); // Warning ON                                    
       Serial.println("senser issue"); 
      }else{
         // / Control logic
          if(distance1 <= 5 && distance2 <= 5) {
           digitalWrite(relay, HIGH);       // Motor ON
           Serial.println("Motor ON");
           digitalWrite(relaystats, HIGH);
          }else {
           digitalWrite(relay, LOW);        // Motor OFF
           Serial.println("Motor OFF");
           digitalWrite(relaystats, LOW);
          }
          digitalWrite(senserled, LOW); // Warning of
     } 
   }
 }
 delay(500); // Delay for stability
}