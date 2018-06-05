#include <Arduino.h>

class Thermistor{
private:
  // define pins numbers
  int sensor_pin; 
  
public:
  Thermistor(int sensor_pin);
  void init();
  float ReadValue();
  
};

