#include "Thermistor.h"
#include "Temperature_Table.h"

Thermistor::Thermistor(int sensor)
:sensor_pin(sensor)
{
    init();
}

void Thermistor::init(void){
    pinMode(sensor_pin, INPUT);
}

float Thermistor::ReadValue(){
  int sensorValue = analogRead(sensor_pin);
  int len = 64;
  int mid = len / 2;
  len = len / 2;
  while(mid!=1){
    mid /= 2;
    if(temptable[len][0]<sensorValue)
      len += mid;
    else if(temptable[len][0]>sensorValue)
      len -= mid;
  }
  float temp = temptable[len-1][1] + ((sensorValue-temptable[len-1][0])*(temptable[len][1]-temptable[len-1][1])/(temptable[len][0]-temptable[len-1][0]));
  return temp;
}

