// LEONARDO !!!!!!!!


#include <ServoEasing.hpp>

ServoEasing myServo;  // Create an instance of the Servo class to control the servo motor

int buttonAPin = A3;  // Pin for button A
int buttonBPin = A2;  // Pin for button B
int buttonCPin = A1;  // Pin for button C

int ledAPin = 3;  // Pin for LED button A
int ledBPin = 5;  // Pin for LED button B
int ledCPin = 6;  // Pin for LED button C

int magnetServoReachPin = A8;
int magnetHomePin = A6;
int verticalPositionPin = A0;

bool isBlinkingA = false;
bool isBlinkingB = false;
bool isBlinkingC = false;
extern bool movementDone = false;

unsigned long previousMillisA = 0;
unsigned long previousMillisB = 0;
unsigned long previousMillisC = 0;

unsigned long time_now = 0;

const long interval = 200;  // LED blink interval (in milliseconds)

void ServoTargetPositionReachedHandler(ServoEasing *aServoEasingInstance);

void setup() {
  myServo.attach(10, 30);  //Servo motor assignment to pin 10

  pinMode(buttonAPin, INPUT);
  pinMode(buttonBPin, INPUT);
  pinMode(buttonCPin, INPUT);

  pinMode(magnetServoReachPin, INPUT);
  pinMode(magnetHomePin, INPUT);
  pinMode(verticalPositionPin, INPUT);

  pinMode(ledAPin, OUTPUT);
  pinMode(ledBPin, OUTPUT);
  pinMode(ledCPin, OUTPUT);



  myServo.setSpeed(60);  // This speed is taken if no further speed argument is given.
  myServo.setTargetPositionReachedHandler(ServoTargetPositionReachedHandler);
}

void loop() {
  if (digitalRead(magnetHomePin) == HIGH) {
    startBlinkingAll();
  } else {
    handleButton(3, buttonCPin, ledCPin, isBlinkingC, previousMillisC, movementDone);    
    handleButton(2, buttonBPin, ledBPin, isBlinkingB, previousMillisB, movementDone);
    handleButton(1, buttonAPin, ledAPin, isBlinkingA, previousMillisA, movementDone);
  }

}

void handleButton(int id, int buttonPin, int ledPin, bool &isBlinking, unsigned long &previousMillis, bool &movementDone) {
  if (digitalRead(buttonPin) == LOW) {
    if (!isBlinking) {
      startBlinking(ledPin, isBlinking, previousMillis);
      rotateServo(id, movementDone);  // Turn the servo motor by 1x
    }
  } else {
    stopBlinking(ledPin, isBlinking);
  }
}

void rotateServo(int rotations, bool &movementDone) {
  for (int i = 0; i < rotations; ++i) {
    movementDone = false;
    myServo.startEaseTo(115);                 // Rotate to position 115 degrees
    HandleBreakableDelay(1500, movementDone);  // Pause
    movementDone = false;
    myServo.startEaseTo(30);                  // Rotate to position 30 degrees
    HandleBreakableDelay(400, movementDone);  // Pause
  }
}

void startBlinking(int ledPin, bool &isBlinking, unsigned long &previousMillis) {
  isBlinking = true;
  previousMillis = millis();
  analogWrite(ledPin, 16);  // LED on
}

void stopBlinking(int ledPin, bool &isBlinking) {
  isBlinking = false;
  analogWrite(ledPin, 0);  // LED off
}


void HandleBreakableDelay(unsigned long msecondsToDelay, bool &movementDone) {
  while (movementDone == false) {
    if (digitalRead(magnetHomePin) == HIGH) {
      // Serial.println("magnetServoReachPin = LOW");
      myServo.stop();
      ErrorEndLoop();
      break;
    } else {
      //  Serial.println("magnetServoReachPin = HIGH");
    }
    delay(10);
  }
  time_now = millis();
  //Serial.println("millis ");
  while (millis() < (time_now + msecondsToDelay)) {
    //wait approx. [period] ms    
    delay(10);
  }
}

void ServoTargetPositionReachedHandler(ServoEasing *aServoEasingInstance) {
  movementDone = true;
}
void ErrorEndLoop() {
  myServo.startEaseTo(30); 
  while (true) {

    startBlinkingAll();

  }
}

void startBlinkingAll() {
  analogWrite(ledAPin, 16);
  analogWrite(ledBPin, 16);
  analogWrite(ledCPin, 16);
  delay(400);
  analogWrite(ledAPin, 0);
  analogWrite(ledBPin, 0);
  analogWrite(ledCPin, 0);
  delay(400);
}