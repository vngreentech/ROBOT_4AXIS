#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte diachi[6] = "12345";

  /*
  * nrf24l01: IRQ-D8 CE-D9 CSN-D10 MOSI-D11 MISO-D12 CLK-13
  * button: A~D2 B~D3 C~D4 D~D5 E~D6 F~D7 X~A0 Y~A1 Key_Joystick~D8
  */
#define x A0
#define y A1
#define joy 8
#define a 2
#define b 3 //Auto home
#define c 4
int gt_x; int gt_y;
byte gt_a; byte gt_b; byte gt_c; byte gt_joy; 

byte gui[30]; //0-255

void setup() 
{
  Serial.begin(115200);

  pinMode(x, INPUT);
  pinMode(y, INPUT);
  pinMode(joy, INPUT_PULLUP);
  pinMode(a, INPUT_PULLUP);
  pinMode(b, INPUT_PULLUP); //Auto home
  pinMode(c, INPUT_PULLUP);

  if (!radio.begin()) 
  {
    Serial.println("Module không khởi động được...!!");
    while (1) {}
  }   

  radio.openWritingPipe(diachi);
  radio.setPALevel(RF24_PA_MIN); //Cài bộ khuyết địa công suất ở mức MIN
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);   
  radio.stopListening(); //Ngưng nhận

  if (!radio.available()) //khong ket noi
  {
    Serial.println("Chưa kết nối được với RX...!!");
    Serial.println("CHỜ KẾT NỐI.......");
  } 
}

void loop() 
{
  gt_b = digitalRead(b); //Auto home
  gui[0] = gt_b;

  gt_a = digitalRead(a);
  gui[3] = gt_a;
  gt_c = digitalRead(c);
  gui[4] = gt_c;
  gt_joy = digitalRead(joy);
  gui[5] = gt_joy;

  gt_x = analogRead(x); //X
  gui[1] = map(gt_x, 0, 1023, 0, 100);
  gt_y = analogRead(y); //Y
  gui[2] = map(gt_y, 0, 1023, 0, 100);

  radio.write(&gui, sizeof(gui)); //Gui du lieu  

  Serial.print("Home= "); Serial.print(gui[0]);
  Serial.print(" - X= "); Serial.print(gui[1]); Serial.print(" - Y= "); Serial.print(gui[2]);
  Serial.print(" - A= "); Serial.print(gui[3]); Serial.print(" - C= "); Serial.print(gui[4]);
  Serial.print(" - JOY= "); Serial.print(gui[5]);

  Serial.println();
  delay(10);
}