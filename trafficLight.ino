/*********************************/
/***LIBRARY***/
#include <Arduino.h>
#include <OneButton.h>
#include "Timer.h"

/*********************************/
/***DEFINE & CONST VALUE***/
#define RED_PIN 4
#define YELLOW_PIN 3
#define GREEN_PIN 2
#define RED_PEDESTRIAN_PIN 7
#define GREEN_PEDESTRIAN_PIN 6
#define BUTTON_PIN 13

const unsigned long TIME_RED                     = 15000;
const unsigned long TIME_YELLOW                  = 5000;
const unsigned long TIME_GREEN                   = 20000;
const unsigned long TIME_PEDESTRIAN_GREEN        = 10000;
const unsigned long TIME_BLINK_YELLOW            = 5000;
const unsigned long TIME_PEDESTRIAN_BLINK_GREEN  = 5000;

/*********************************/
/***GLOBAL VARIABLE***/
OneButton BTN = OneButton(BUTTON_PIN, true, true);

enum lightsState {
  RED,
  YELLOW,
  GREEN,
  BLINK_YELLOW,
};

enum pedestrianState {
  STOP, 
  GO,
  BLINK,
};

lightsState currentState;
pedestrianState pCurrentState;

Timer timer;
bool isPress = false;

/*********************************/
/***LIBRARY***/
void setup() {
  Serial.begin(9600);
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(GREEN_PEDESTRIAN_PIN, OUTPUT);
  pinMode(RED_PEDESTRIAN_PIN, OUTPUT);

  BTN.attachClick(pedestrian);

  OFF();
  pedestrianOFF();

  currentState = RED;
  timer.startTimer(TIME_RED);
  pCurrentState = GO;
  timer.startPedestrianTimer(TIME_PEDESTRIAN_GREEN);
}

/*********************************/
/***MAIN***/
void loop() {
  BTN.tick();

  if (isPress) {
    need2Cross();
  }
  trafficLights();
  pedestrainLights();
}

/*********************************/
/***TRAFFIC LIGHTS STATE TRANSITION***/
void trafficLights() {
  switch (currentState) {
    case RED:
      red();
      if (timer.isDone()) {
        currentState = GREEN;
        timer.startTimer(TIME_GREEN);
        Serial.print("TIME: - ");
        Serial.print(timer.timerStart);
        Serial.print(" RED");
        Serial.println(" --> GREEN");
      }
      break;
    case YELLOW:
      yellow();
      if (timer.isDone()) {
        currentState = RED;
        timer.startTimer(TIME_RED);
        Serial.print("TIME: - ");
        Serial.print(timer.timerStart);
        Serial.print(" YELLOW");
        Serial.println(" --> RED");
      }
      break;
    case GREEN:
      green();
      if (timer.isDone()) {
        currentState = YELLOW;
        timer.startTimer(TIME_YELLOW);
        Serial.print("TIME: - ");
        Serial.print(timer.timerStart);
        Serial.print(" GREEN");
        Serial.println(" --> YELLOW");
      }
      break;
    case BLINK_YELLOW:
      blinkYellow();
      if (timer.isBlinkDone()) {
        currentState = RED;
        timer.startTimer(TIME_RED);
        Serial.print("TIME: - ");
        Serial.print(timer.timerStart);
        Serial.print(" BLINK YELLOW");
        Serial.println(" --> RED");
      }
      break;
  }
}

void pedestrainLights() {
  switch(pCurrentState) {
    case STOP:
      redPedestrian();
      if (currentState == RED) {
        pCurrentState = GO;
        timer.startPedestrianTimer(TIME_PEDESTRIAN_GREEN);
      }
      break;
    case GO:
      greenPedestrian();
      if (timer.isPedestrianDone()) {
        pCurrentState = BLINK;
        timer.startBlinkTimer(TIME_PEDESTRIAN_BLINK_GREEN);
      }
      break;
    case BLINK:
      blinkGreenPedestrian();
      if (timer.isBlinkDone()) {
        pCurrentState = STOP;
      }
      break;
  }
}

/*********************************/
/***PEDESTRIAN FUNCTION***/
void pedestrian() {
  if (currentState == GREEN) {
    isPress = true;
    OFF();
    currentState = BLINK_YELLOW;
    timer.startBlinkTimer(TIME_BLINK_YELLOW);
  }
}

void need2Cross() {
  if (timer.isBlinkDone()) {
    pedestrianOFF();
    pCurrentState = GO;
    timer.startPedestrianTimer(TIME_PEDESTRIAN_GREEN);
    isPress = false;
  }
}

/*********************************/
/***BLINK LIGHTS FUNCTION***/
//ON 500ms OFF 500ms
void blinkYellow() {
  if (millis() % 1000 > 500) {
    yellow();
  } else {
    OFF();
  }
}

//ON 500ms OFF 500ms
void blinkGreenPedestrian() {
  if (millis() % 1000 > 500)  {
    greenPedestrian();
  } else {
    pedestrianOFF();
  }
}

/*********************************/
/***ON/OFF FUNCTION***/
void OFF() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}

void pedestrianOFF() {
  digitalWrite(GREEN_PEDESTRIAN_PIN, LOW);
  digitalWrite(RED_PEDESTRIAN_PIN, LOW);
}

void red() {
  OFF();
  digitalWrite(RED_PIN, HIGH);
}

void yellow() {
  OFF();
  digitalWrite(YELLOW_PIN, HIGH);
}

void green() {
  OFF();
  digitalWrite(GREEN_PIN, HIGH);
}

void redPedestrian() {
  pedestrianOFF();
  digitalWrite(RED_PEDESTRIAN_PIN, HIGH);
}

void greenPedestrian() {
  pedestrianOFF();
  digitalWrite(GREEN_PEDESTRIAN_PIN, HIGH);
}