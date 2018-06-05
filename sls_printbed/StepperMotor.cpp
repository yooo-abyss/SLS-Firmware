#include "StepperMotor.h"

StepperMotor::StepperMotor(int steps, int dir_pin, int step_pin)
:motor_steps_per_rev(steps), dir_pin(dir_pin), step_pin(step_pin)
{
    init();
}

StepperMotor::StepperMotor(int steps, int dir_pin, int step_pin, int enable_pin)
:motor_steps_per_rev(steps), dir_pin(dir_pin), step_pin(step_pin), enable_pin(enable_pin)
{
    init();
}

StepperMotor::StepperMotor(int steps, float pitch, int dir_pin, int step_pin, int enable_pin)
:motor_steps_per_rev(steps), mm_per_rev(pitch), dir_pin(dir_pin), step_pin(step_pin), enable_pin(enable_pin)
{
    init();
}

void StepperMotor::init(void){
    pinMode(dir_pin, OUTPUT);
    digitalWrite(dir_pin, HIGH);

    pinMode(step_pin, OUTPUT);
    digitalWrite(step_pin, LOW);

    if IS_CONNECTED(enable_pin){
        pinMode(enable_pin, OUTPUT);
        digitalWrite(enable_pin, HIGH); // disable
    }
    enable();
}

void StepperMotor::Recalculate(){
  this->steps_per_rev = this->motor_steps_per_rev * this->microstep;
  this->steps_per_mm = this->steps_per_rev / this->mm_per_rev;
  this->pulse_width = 1000000 / (this->steps_per_mm * this->speed );
}

void StepperMotor::SetDirection(int move_dir){
  int actual_dir = move_dir * this->dir;
  if(actual_dir == 1)
    digitalWrite(this->dir_pin, LOW);
  else if(actual_dir == -1)
    digitalWrite(this->dir_pin, HIGH);
  else
    Serial.println("Input Parameter is not in expected format");
}

void StepperMotor::move(float mm){
  this->loc += mm;
  long steps = this->steps_per_mm * mm;
  if (steps >= 0){
      SetDirection(1);
  } else {
      SetDirection(-1);
      steps = -steps;
  }
  /*
   * We currently try to do a 50% duty cycle so it's easy to see.
   * Other option is step_high_min, pulse_duration-step_high_min.
   */
  unsigned long pulse_duration = this->pulse_width/2;
  while (steps--){
      digitalWrite(step_pin, HIGH);
      unsigned long next_edge = micros() + pulse_duration;
      microWaitUntil(next_edge);
      digitalWrite(step_pin, LOW);
      microWaitUntil(next_edge + pulse_duration);
  }
}

void StepperMotor::enable(void){
    if IS_CONNECTED(enable_pin){
        digitalWrite(enable_pin, LOW);
    }
}

void StepperMotor::SetMicrostep(int val){
  this->microstep = val;
  Recalculate();
}

void StepperMotor::SetRPM(float val){
  this->speed = val * this->mm_per_rev / 60;
  Recalculate();
}

void StepperMotor::SetSpeed(float val){
  this->speed = val;
  Recalculate();
}

void StepperMotor::PrintStats(){
  Serial.print("Speed ");
  Serial.println(this->speed);
  Serial.print("Location ");
  Serial.println(this->loc);
  Serial.print("Pulse Width ");
  Serial.println(this->pulse_width);
}
