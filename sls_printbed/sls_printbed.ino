#include "StepperMotor.h"
#include "BedHeater.h"
#include "Thermistor.h"

String input,command,value;

StepperMotor small_bed = StepperMotor(200,5,A6,A2,A7);
StepperMotor big_bed = StepperMotor(200,5,A0,38,A1);
BedHeater small_heater = BedHeater(A4);
Thermistor small_thermistor = Thermistor(A13);

void setup() {
  small_bed.SetMicrostep(16);
  big_bed.SetMicrostep(16);
  Serial.begin(9600);
  
}

void loop() {
  if(Serial.available()){
    input = Serial.readString();
    ExtractParameters();
    // M1 - Move small bed
    // M2 - Move big bed
    if(command == "M1"){
      MoveBy(small_bed,value.toFloat());      
    }
    else if (command == "M2")
      MoveBy(big_bed,value.toFloat());
    else if (command == "M11")
      SetSpeed(small_bed,value.toFloat());
    else if (command == "M21")
      SetSpeed(big_bed,value.toFloat());
    else if (command == "M32"){    
      float small_temp = ReadValue(small_thermistor);
      Serial.println(small_temp);   
    }
    else if (command == "M31")
      SetValue(small_thermistor,small_heater, value.toFloat());
    
  }
}

void MoveBy(StepperMotor& motor, float value){
  motor.move(value);
  motor.PrintStats();
}

void SetSpeed(StepperMotor& motor, float value){
  motor.SetSpeed(value);
  motor.PrintStats();
}

float ReadValue(Thermistor& thermistor){
  return thermistor.ReadValue();
}

void SetValue(Thermistor& thermistor,BedHeater& small_heater, float value){
  Serial.print("Setting Temp to ");
  Serial.println(value);
  float temp = thermistor.ReadValue();
  while(temp < value && temp > 10){
    Serial.println(temp);
    small_heater.PowerON();
    temp = thermistor.ReadValue();
    delay(100);
  }
  small_heater.PowerOFF();
}

void ExtractParameters(){
  for (int i = 0; i < input.length(); i++) {
    if (input.substring(i, i+1) == " ") {
      command = input.substring(0, i);
      value = input.substring(i+1).toFloat();
      break;
    }
  }
}
