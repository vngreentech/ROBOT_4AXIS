#include <Arduino.h>
#include <Servo.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#define nutnhan 12
int gt_nutnhan; 
int biendem = 0;
int macdinh = 1;

void setup()
{
    Serial.begin(9600);
    pinMode(nutnhan, INPUT_PULLUP);
}

void loop()
{
  gt_nutnhan = digitalRead(nutnhan);

  if(gt_nutnhan != macdinh)
  {
    if(gt_nutnhan == 0)
      {
        biendem += 1;
      }
    macdinh = gt_nutnhan;
  }

  Serial.print("GTnutnhan: "); Serial.print(gt_nutnhan);  
  Serial.print("  - biendem: "); Serial.print(biendem);
  
  Serial.println();
  delay(100);
}
