#include <Arduino.h>

class BedHeater{
private:
  // define pins numbers
  int relay_pin; 
  
public:
  BedHeater(int ssr);
  void init();
  void PowerON();
  void PowerOFF();
};

