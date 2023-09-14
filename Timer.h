#ifndef _TIMER_H_
#define _TIMER_H_

class Timer {

public:
  unsigned long timerStart = 0;
  unsigned long timerTarget  = 0;
  unsigned long timerPedestrianStart = 0;
  unsigned long timerPedestrianTarget = 0;
  unsigned long timerBlinkStart = 0;
  unsigned long timerBlinkTarget = 0;

  void startTimer(unsigned long timer1) {
    timerStart = millis();
    timerTarget = timer1;
  }

  void startPedestrianTimer(unsigned long timer2) {
    timerPedestrianStart = millis();
    timerPedestrianTarget = timer2;
  }

  void startBlinkTimer(unsigned long timer3) {
    timerBlinkStart = millis();
    timerBlinkTarget = timer3;
  }

  bool isDone() {
    return (millis() - timerStart) >= timerTarget; 
  }

  bool isPedestrianDone() {
    return (millis() - timerPedestrianStart) >= timerPedestrianTarget; 
  }

  bool isBlinkDone() {
    return (millis() - timerBlinkStart) >= timerBlinkTarget; 
  }
};

#endif // _TIMER_H_