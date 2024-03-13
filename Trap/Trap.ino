#include <Servo.h>

static const int ledPins[] = {3,5,6};
static const int ledPinsCount = 3;
static const int servoPin = 11;
static const int piezoPin = A0;

static const int knockThreshold = 2;
static const int quietKnock = 5;
static const int loudKnock = 100;

static const int doorStateClosed = 0;
static const int doorStateOpen = 90;

static const int loopInterval = 50;
static const int doorOpenInterval = 2500;

typedef struct State {
  int knockValue;
  int numberOfKnocks;
  int doorState;
  int doorTimeout;
} State_t;

Servo servo;
State_t state;

int knockVal;
int numberOfKnocks = 0;

boolean isOpen = false;
int doorTimeout = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) { delay(100); }
  delay(500);

  for (int i = 0; i < ledPinsCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  state = {0, 0, doorStateOpen, 0};

  servo.attach(servoPin);
  servo.write(state.doorState);

  delay(100);
  Serial.println("Setup complete");
}

void loop() {
  for (int i = 0; i < ledPinsCount; i++) {
    int brightness = random(0, 256);
    analogWrite(ledPins[i], brightness);
  }

  if (state.doorState == doorStateOpen) {
    state.knockValue = analogRead(piezoPin);
    if (state.numberOfKnocks < knockThreshold && state.knockValue > 0) {
      if (checkForKnock(state.knockValue)) {
        state.numberOfKnocks++;
      }
      Serial.print("Knocks to go: ");
      Serial.println(knockThreshold - state.numberOfKnocks);
    }

    if (state.numberOfKnocks >= knockThreshold) {
      closeDoor();
    }

  } else {
    if (state.doorTimeout < 0) {
      openDoor();
    }
    state.doorTimeout -= loopInterval;
  }

  delay(loopInterval);
}

void openDoor() {
  Serial.println("Opening the door");
  state.doorState = doorStateOpen;
  state.doorTimeout = 0;
  servo.write(state.doorState);
  state.numberOfKnocks = 0;
  delay(500);
}

void closeDoor() {
  Serial.println("Closing the door");
  state.doorState = doorStateClosed;
  state.doorTimeout = doorOpenInterval;
  servo.write(state.doorState);
  state.numberOfKnocks = 0;
  delay(500);
}

boolean checkForKnock(int value) {
  if (value > quietKnock && value < loudKnock) {
    Serial.print("Valid knock of value: ");
    Serial.println(value);
    return true;
  }
  return false;
}
