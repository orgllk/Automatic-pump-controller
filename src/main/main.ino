#include <Arduino.h>

// Ultrasonic sensor pins
#define ultt1 5   // Trigger pin for sensor 1
#define ultr1 6   // Echo pin for sensor 1
#define ultt2 7   // Trigger pin for sensor 2
#define ultr2 8   // Echo pin for sensor 2

// Relay and indicators
#define relay 9
#define relaystats 10
#define relaystatssound 11

void setup() {
  Serial.begin(9600);

  // Ultrasonic pins
  pinMode(ultt1, OUTPUT);
  pinMode(ultr1, INPUT);
  pinMode(ultt2, OUTPUT);
  pinMode(ultr2, INPUT);

  // Relay and indicators
  pinMode(relay, OUTPUT);
  pinMode(relaystats, OUTPUT);
  pinMode(relaystatssound, OUTPUT);
}

void loop() {
  // Trigger sensor 1
  digitalWrite(ultt1, LOW);
  delayMicroseconds(2);
  digitalWrite(ultt1, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultt1, LOW);

  // Read echo from sensor 1
  long duration1 = pulseIn(ultr1, HIGH);
  int distance1 = duration1 * 0.034 / 2;
  Serial.print("Distance1: ");
  Serial.println(distance1);

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
  Serial.println(distance2);

  // Control logic
  if (distance1 <= 41 && distance2 <= 41) {
    digitalWrite(relay, HIGH);       // Motor ON
    Serial.println("Motor ON");
    digitalWrite(relaystats, HIGH);
  } else {
    digitalWrite(relay, LOW);        // Motor OFF
    Serial.println("Motor OFF");
    digitalWrite(relaystats, LOW);
    digitalWrite(relaystatssound, LOW);
  }
}