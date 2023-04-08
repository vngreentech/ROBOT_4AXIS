#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

int diachi_ghi = 645;
byte giatri_ghi = 100; //byte là kiểu số nguyên từ 0-255

int diachi_doc = 645;
byte giatri_doc; //Kiểu số nguyên nằm trong 0-255

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  EEPROM.write(diachi_ghi, giatri_ghi);

  giatri_doc = EEPROM.read(diachi_doc);

  Serial.print("Địa chỉ: "); Serial.print(diachi_doc); Serial.print("   ");
  Serial.print("Giá trị: "); Serial.println(giatri_doc);

  delay(100);
}