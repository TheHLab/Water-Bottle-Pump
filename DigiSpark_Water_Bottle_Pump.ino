#include <VL53L0X.h>
#include "Wire.h"

#define NOISE 5
#define STOP_TIME 1500
#define RANGE_ON 30

VL53L0X sensor;
uint16_t distance = 0;
int count_on = 0;
int count_off = 0;
bool pumping = false;
bool just_pumping = false;

void setup()
{
  Wire.begin();

  sensor.setTimeout(500);
  if (!sensor.init())
  {
   while (1) {}
  }
  sensor.startContinuous();

  pinMode(1, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop()
{
  distance = sensor.readRangeContinuousMillimeters();
  if( distance > 0 ){
    if( distance < RANGE_ON ){
      count_on++;
    } else {
      count_off++;
    }
    if( count_on > NOISE || count_off > NOISE ){
      // trigger...
      pumping = false;
      if( count_on > NOISE ) pumping = true;
      count_on = 0;
      count_off = 0;
    }
  }
  if( pumping ){
    digitalWrite(1, HIGH); // LED indicator & pumping motor
    just_pumping = true;
  } else if( just_pumping ) {
    just_pumping = false;
    digitalWrite(1, LOW);
    // open solenoid
    digitalWrite(4, HIGH);
    delay(STOP_TIME); // 
    digitalWrite(4, LOW);
  }
}
