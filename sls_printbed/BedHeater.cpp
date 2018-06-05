#include "BedHeater.h"

BedHeater::BedHeater(int ssr)
:relay_pin(ssr)
{
    init();
}

void BedHeater::init(void){
    pinMode(relay_pin, OUTPUT);
    digitalWrite(relay_pin, LOW);
}

void BedHeater::PowerON(){
  digitalWrite(relay_pin, HIGH);
}

void BedHeater::PowerOFF(){
  digitalWrite(relay_pin, LOW);
}

