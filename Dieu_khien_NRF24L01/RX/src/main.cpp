#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte diachi[6] = "12345";

// nrf24l01: IRQ-D8 CE-D9 CSN-D10 MOSI-D11 MISO-D12 SCK-13

byte nhan[30];

Servo servo_A;
Servo servo_B;
Servo servo_Z;
Servo servo_kep;

#define servoA  6
#define servoB  5
#define servoZ  7 
#define servokep  8

byte macdinh = 1; int dem=0;

void auto_home()
{
  servo_A.write(50);
  servo_B.write(50);
  servo_Z.write(90);
  servo_kep.write(0);
}

void truc_z()
{
  byte goc_z; byte cong; byte tru;
  if(nhan[1]>=50 && nhan[1]<=100) //Phai
  {
    goc_z = servo_Z.read(); //Doc goc Servo Z
    if(goc_z < 180)
    {
      cong = map(nhan[1], 50, 99, 0, 1);
      goc_z = goc_z + cong;
      servo_Z.write(goc_z);
      tru = 0;
    }
    else //Neu goc_Z > 180
    {
      cong = 0; tru = 0;
      servo_Z.write(180);
    }
  }
  else //Trai.....0-49
  {
    goc_z = servo_Z.read(); //doc goc Servo Z
    if(goc_z > 0) //lon hon 0
    {
      tru = map(nhan[1], 49, 0, 0, 1);
      goc_z = goc_z - tru;
      servo_Z.write(goc_z);
      cong = 0;
    }
    else //nho hon 0
    {
      cong = 0; tru = 0;
      servo_Z.write(0);
    }
  }

  Serial.print("Z: "); Serial.print(servo_Z.read());
}

void truc_A()
{
  byte goc_a; byte cong; byte tru;
  if(nhan[2]>=50 && nhan[2]<=100) //Len
  {
    goc_a = servo_A.read(); //Doc goc Servo A
    if(goc_a < 80)
    {
      cong = map(nhan[2], 50, 99, 0, 1);
      goc_a = goc_a + cong;
      servo_A.write(goc_a);
      tru = 0;
    }
    else //Neu goc_A > 80
    {
      cong = 0; tru = 0;
      servo_A.write(80);
    }
  }
  else //Xuong.....0-49
  {
    goc_a = servo_A.read(); //doc goc Servo A
    if(goc_a > 50) //lon hon 50
    {
      tru = map(nhan[2], 49, 0, 0, 1);
      goc_a = goc_a - tru;
      servo_A.write(goc_a);
      cong = 0;
    }
    else //nho hon 50
    {
      cong = 0; tru = 0;
      servo_A.write(50);
    }
  }  

  Serial.print(" - A: "); Serial.print(servo_A.read());
}

void truc_B()
{
  byte goc_B;
  goc_B = servo_B.read();
  if(nhan[3] == 0) //Nut A
  {
    if(goc_B < 120)
    {
      goc_B = goc_B + 1;
      servo_B.write(goc_B);
    }
    else  servo_B.write(120);
  }

  if(nhan[4] == 0) //Nut C
  {
    if(goc_B > 50)
    {
      goc_B = goc_B - 1;
      servo_B.write(goc_B);
    }
    else  servo_B.write(50);
  }  

  Serial.print(" - B: "); Serial.print(servo_B.read());
} 

void setup() 
{
  Serial.begin(115200);

  servo_A.attach(servoA); //50-80
  servo_B.attach(servoB); //50-120
  servo_Z.attach(servoZ);
  servo_kep.attach(servokep); //180 - kep

  if (!radio.begin()) 
  {
    Serial.println("Module không khởi động được...!!");
    while (1) {}
  }   

  radio.openReadingPipe(1, diachi);
  radio.setPALevel(RF24_PA_MIN); //Cài bộ khuyết địa công suất ở mức MIN
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);   
  radio.startListening(); //Bta dau nhan

  if (!radio.available()) //khong ket noi
  {
    Serial.println("Chưa kết nối được với RX...!!");
    Serial.println("CHỜ KẾT NỐI.......");
  }   

  auto_home();
}

void loop() 
{
  if (radio.available()) 
  {
    radio.read(&nhan, sizeof(nhan));
  }

  if(nhan[0] == 0) //nhan nut auto home | nut B
    auto_home();
  else 
  {
    if(nhan[5] != macdinh)
    {
      if(nhan[5] == 0)
        dem++;
      macdinh = nhan[5];
    }
    if(dem%2 == 0)
      servo_kep.write(180); //kep
    else servo_kep.write(0); //mo

    truc_z();
    truc_A();
    truc_B();
  }

  Serial.print(" | Home= "); Serial.print(nhan[0]); Serial.print(" - X= "); Serial.print(nhan[1]); 
  Serial.print(" - Y= "); Serial.print(nhan[2]); Serial.print(" - A= "); Serial.print(nhan[3]); 
  Serial.print(" - C= "); Serial.print(nhan[4]); Serial.print(" - JOY= "); Serial.print(nhan[5]);

  Serial.println();  
  delay(30);
}