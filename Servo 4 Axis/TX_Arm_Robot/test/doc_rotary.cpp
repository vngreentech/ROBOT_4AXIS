#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int phaA = A2; int phaB = A3;
int menu = 7; int gtmenu;
int dem_rota = 0; 
int hientai; int bandau; 
int gtphaA = LOW; int phaA_last = LOW;
int dem_menu = 0;

void setup() 
{
  Serial.begin(9600);

  pinMode(phaA,INPUT);
  pinMode(phaB,INPUT);
  pinMode(menu, INPUT_PULLUP);    
   
  bandau = digitalRead(phaA);
}

void loop()
{
    gtmenu = digitalRead(menu);
    if(gtmenu == 0)
      dem_menu ++;
    //Serial.print("Menu: "); Serial.println(dem_menu);

    // gtphaA = digitalRead(phaA); 
    hientai = digitalRead(phaA);
  
  // if ((phaA_last == LOW) && (gtphaA == HIGH))
  if (hientai != bandau)
   {     
    //if (digitalRead(phaB) == LOW)  // + XOAY NGUOC CHIEU
    if (digitalRead(phaB) != hientai) //+
    {
        dem_rota ++;
    }
    else  // - XOAY CUNG CHIEU
    {
        dem_rota --;
    }
    Serial.print("  Dem rota: "); Serial.println(dem_rota);
   }
   bandau = hientai;
  //  phaA_last = gtphaA;
}