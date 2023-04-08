#include <Arduino.h>
#include <Servo.h>

Servo servo_A;
Servo servo_B;
Servo servo_Z;
Servo servo_kep;

#define servoA  8
#define servoB  9
#define servoZ  10 
#define servokep  11

#define nutnhan 12
#define bientro_A A0
#define bientro_B A1
#define bientro_Z A2

int gt_bientro_A; int gt_bientro_B; int gt_bientro_Z;
int goc_A;  int goc_B;  int goc_Z;
byte gt_nutnhan; byte macdinh = 1; int biendem = 0;

void setup() 
{
  Serial.begin(115200);
  servo_A.attach(servoA);
  servo_B.attach(servoB);
  servo_Z.attach(servoZ);
  servo_kep.attach(servokep);

  pinMode(bientro_A, INPUT);
  pinMode(bientro_B, INPUT);
  pinMode(bientro_Z, INPUT);
  pinMode(nutnhan, INPUT_PULLUP);
}

void loop() 
{
  gt_nutnhan = digitalRead(nutnhan);
  if(gt_nutnhan != macdinh)
  {
    if(gt_nutnhan == 0)
      biendem ++;
    macdinh = gt_nutnhan;
  }
  if(biendem % 2 == 0) servo_kep.write(180); //Kep
  else  servo_kep.write(0); //Mo

  gt_bientro_A = analogRead(bientro_A); //Doc bien tro A
  goc_A = map(gt_bientro_A, 0, 1023, 50, 80); //chuyen doi sang goc Servo tu 0-180' | 50-80
  servo_A.write(goc_A); //Dieu khien Servo A

  gt_bientro_B = analogRead(bientro_B);
  goc_B = map(gt_bientro_B, 0, 1023, 50, 120); //50-120
  servo_B.write(goc_B);

  gt_bientro_Z = analogRead(bientro_Z);
  goc_Z = map(gt_bientro_Z, 0, 1023, 0, 180);    
  servo_Z.write(goc_Z);
  
  Serial.print("BT_A: "); Serial.print(gt_bientro_A); Serial.print(" - GOC A: "); Serial.print(goc_A);
  Serial.print("  | BT_B: "); Serial.print(gt_bientro_B); Serial.print(" - GOC B: "); Serial.print(goc_B);
  Serial.print("  | BT_Z: "); Serial.print(gt_bientro_Z); Serial.print(" - GOC Z: "); Serial.print(goc_Z);
  Serial.print("  | biendem: "); Serial.print(biendem);
  
  Serial.println();
  delay(50);
}