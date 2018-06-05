#include <Arduino.h>


// used internally by the library to mark unconnected pins
#define PIN_UNCONNECTED -1
#define IS_CONNECTED(pin) (pin != PIN_UNCONNECTED)
inline void microWaitUntil(unsigned long target_micros){
  yield();
  while (micros() < target_micros);
}

class StepperMotor{
private:
  // define pins numbers
  int step_pin; 
  int dir_pin; 
  int enable_pin = -1;
  
  // define Constants
  unsigned int motor_steps_per_rev = 200;
  unsigned int microstep = 1;
  float mm_per_rev;
  float speed = mm_per_rev;  // speed in mm/s
  unsigned int steps_per_rev = motor_steps_per_rev * microstep;
  float steps_per_mm = steps_per_rev / mm_per_rev;
  unsigned long int pulse_width = 1000000 * speed / steps_per_mm;  // pulse width in microseconds
  float loc; // Motor Location in mm
  int dir = 1;  // LOW means CW and HIGH means CCW, if CW means positive movement dir = 1, if CW means negative movement then dir = -1

  void init(void);
  void SetDirection(int val);
  void Recalculate();
  
public:
  StepperMotor(int steps, float pitch, int dir_pin, int step_pin, int enable_pin);
  StepperMotor(int steps, int dir_pin, int step_pin, int enable_pin);
  StepperMotor(int steps, int dir_pin, int step_pin);
  void SetRPM(float val);
  void SetSpeed(float val); // Speed in mm/s
  void SetMicrostep(int val);
  void SetHomeLocation(float loc);
  void SetHome();
  void SetMovementDirection(int val);
  void move(float val);
  void enable();
  void PrintStats();
};

