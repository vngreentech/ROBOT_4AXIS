#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte diachi[6] = "12345"; //Mảng kí tự dạng chuỗi có 6 kí tự

  /*
  * nrf24l01: IRQ-D8 CE-D9 CSN-D10 MOSI-D11 MISO-D12 CLK-13
  * button: A~D2 B~D3 C~D4 D~D5 E~D6 F~D7 X~A0 Y~A1 Key_Joystick~D8
  */

#define A 2
#define B 3
#define C 4
#define D 5
#define E 6
#define joy 8

#define X A0
#define Y A1

byte gui[10];
int gocX; int gocY;

void setup() 
{
  Serial.begin(9600);

  pinMode(A, INPUT_PULLUP);
  pinMode(B, INPUT_PULLUP);
  pinMode(C, INPUT_PULLUP);
  pinMode(D, INPUT_PULLUP);
  pinMode(E, INPUT_PULLUP);
  pinMode(joy, INPUT_PULLUP);

 if (!radio.begin()) 
  {
    Serial.println("Module không khởi động được...!!");
    while (1) {}
  }   
  radio.openWritingPipe(diachi);
  radio.setPALevel(RF24_PA_MIN); //Cài bộ khuyết địa công suất ở mức MIN
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);  
  radio.stopListening(); //Cài đặt module là TX  

  if (!radio.available())
  {
    Serial.println("Chưa kết nối được với RX...!!");
    Serial.println("CHỜ KẾT NỐI.......");
  }  
}

void loop() 
{
  gui[0] = digitalRead(A); 
  gui[1] = digitalRead(B);
  gui[2] = digitalRead(C); 
  gui[3] = digitalRead(D);
  gui[4] = digitalRead(E); 
  gui[5] = digitalRead(joy);

  gocX = analogRead(X);
  gui[6] = map(gocX, 0, 1023, 0, 100);
  gocY = analogRead(Y);
  gui[7] = map(gocY, 0, 1023, 0, 100);

  Serial.print("A: "); Serial.print(gui[0]); 
  Serial.print("  B: "); Serial.print(gui[1]);
  Serial.print("  C: "); Serial.print(gui[2]);
  Serial.print("  D: "); Serial.print(gui[3]);
  Serial.print("  E: "); Serial.print(gui[4]);
  Serial.print("  JOY: "); Serial.print(gui[6]);

  Serial.print(" --- X: "); Serial.print(gui[7]);
  Serial.print(" --- Y: "); Serial.println(gui[8]);

  radio.write(&gui, sizeof(gui)); //Gui du lieu

  delay(30);
}