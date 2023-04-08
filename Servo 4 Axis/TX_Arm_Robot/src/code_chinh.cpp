#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

byte a; byte b; byte c; byte d; byte e; byte JOY; byte x; byte y;
byte macdinh = 1; 
int demmenu = 0; int congtru_chinh = 0;
unsigned int congtru_xoa = 0;

RF24 radio(9, 10); // CE, CSN
const byte diachi[][6] = {"12345", "10000"}; //0, 1
//const byte diachi[6] = "12345"; //Mảng kí tự dạng chuỗi có 6 kí tự

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

byte gui[30];
byte nhan[30];
int gocX; int gocY;
byte control = 0; unsigned long vitri = 0;
float delay_vitri = 0.1; float delay_lenh = 0.1;
int floattoint;
byte time_vitri; byte time_lenh;
byte tong_lenh = 0;
byte chon_lenh = 1;
unsigned int solan_chay = 0;
unsigned long  time1 = 0;
unsigned int lenhxoa = 1;
bool kiemtra = true; //bien kiem tra nut menu

void manhinh()  // HIỂN THỊ MÀN HÌNH CHÍNH
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A1: ");  
  lcd.setCursor(7, 0);
  lcd.print("A2: ");
  lcd.setCursor(14, 0);
  lcd.print("A3: "); 

  lcd.setCursor(0, 1); //Lenh dang thuc hien
  lcd.print("LENH: ");
  lcd.setCursor(9, 1); // so lan thuc hien; VD: 2/10
  lcd.print("SL: ");  
  if(gui[14] == 3) //dang thuc hien lenh
  {
    lcd.setCursor(4, 3);
    lcd.print("[OK] -> STOP");    
    control = 1; //DIEU KHIEN OFF
    lcd.setCursor(5, 1);
    lcd.print(chon_lenh);
    if(solan_chay == 0)
    {
      lcd.setCursor(12, 1);
      lcd.print("ALWAYS");
    } 
    else 
    {
      lcd.setCursor(12, 1);
      lcd.print(solan_chay);
    }    
  }  
  else //KHONG co thuc hien lenh nao
  {
    lcd.setCursor(5, 1);
    lcd.print("---");
    lcd.setCursor(12, 1);
    lcd.print("---");
  }

  lcd.setCursor(0, 2); // Cho phep dieu khien tu TX hoac khong
  lcd.print("Control: ");  // YES / NO  

}

void menu_chinh()
{
  if (congtru_chinh == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">BACK");
    lcd.setCursor(0, 1);
    lcd.print(" AUTO HOME");  
    lcd.setCursor(0, 2);
    lcd.print(" LENH DIEU KHIEN");
    lcd.setCursor(0, 3);
    lcd.print(" THEM LENH");        
  }
  else if (congtru_chinh == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" BACK");
    lcd.setCursor(0, 1);
    lcd.print(">AUTO HOME");  
    lcd.setCursor(0, 2);
    lcd.print(" LENH DIEU KHIEN");
    lcd.setCursor(0, 3);
    lcd.print(" THEM LENH");     
  }
  else if (congtru_chinh == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" BACK");
    lcd.setCursor(0, 1);
    lcd.print(" AUTO HOME");  
    lcd.setCursor(0, 2);
    lcd.print(">LENH DIEU KHIEN");
    lcd.setCursor(0, 3);
    lcd.print(" THEM LENH");  
  }
  else if (congtru_chinh == 3)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" BACK");
    lcd.setCursor(0, 1);
    lcd.print(" AUTO HOME");  
    lcd.setCursor(0, 2);
    lcd.print(" LENH DIEU KHIEN");
    lcd.setCursor(0, 3);
    lcd.print(">THEM LENH");  
  }
  else if (congtru_chinh == 4)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" AUTO HOME");  
    lcd.setCursor(0, 1);
    lcd.print(" LENH DIEU KHIEN");
    lcd.setCursor(0, 2);
    lcd.print(" THEM LENH");
    lcd.setCursor(0, 3);
    lcd.print(">XOA LENH");        
  }    
}

void ht_control() //Hien thi ON/OFF Control man hinh chinh
{
  if(demmenu == 0)
  {
    if(control == 1) //OFF ngat dieu khien
    {
      gui[9] = 1;      
      lcd.setCursor(9, 2);
      lcd.print("OFF");    
    }
    else // = 0 _ ON dieu khien 
    {
      gui[9] = 0;
      lcd.setCursor(9, 2);
      lcd.print("ON ");    
    } 
  }
}

void ht_goc() //Hien thi cac goc
{
  if(demmenu == 0 && congtru_chinh == 0) //hien thi man hinh chinh
  {
    lcd.setCursor(3, 0); //A
    lcd.print(nhan[0]);

    if(nhan[1] < 100) //B
    {
      lcd.setCursor(10, 0); //B
      lcd.print(nhan[1]);
      lcd.setCursor(12, 0);
      lcd.print(" ");      
    }
    else 
    {
      lcd.setCursor(10, 0); //B
      lcd.print(nhan[1]);
    }

    if(nhan[2] < 100) //C
    {
      lcd.setCursor(17, 0); //C
      lcd.print(nhan[2]);
      lcd.setCursor(19, 0);
      lcd.print(" ");      
    }
    else 
    {
      lcd.setCursor(17, 0); //C
      lcd.print(nhan[2]);
    }
    if(nhan[2] < 10)
    {
      lcd.setCursor(17, 0); //C
      lcd.print(nhan[2]);
      lcd.setCursor(18, 0);
      lcd.print("  ");
    }    
  }

  if(demmenu == 2 && congtru_chinh == 3) //hien thi THEM LENH
  {
    lcd.setCursor(0, 2);
    lcd.print("A1: ");  
    lcd.setCursor(7, 2);
    lcd.print("A2: ");
    lcd.setCursor(14, 2);
    lcd.print("A3: ");     

    lcd.setCursor(3, 2); //A
    lcd.print(nhan[0]);

    if(nhan[1] < 100) //B
    {
      lcd.setCursor(10, 2); //B
      lcd.print(nhan[1]);
      lcd.setCursor(12, 2);
      lcd.print(" ");      
    }
    else 
    {
      lcd.setCursor(10, 2); //B
      lcd.print(nhan[1]);
    }

    if(nhan[2] < 100) //C
    {
      lcd.setCursor(17, 2); //C
      lcd.print(nhan[2]);
      lcd.setCursor(19, 2);
      lcd.print(" ");      
    }
    else 
    {
      lcd.setCursor(17, 2); //C
      lcd.print(nhan[2]);
    }
    if(nhan[2] < 10)
    {
      lcd.setCursor(17, 2); //C
      lcd.print(nhan[2]);
      lcd.setCursor(18, 2);
      lcd.print("  ");
    }
  }
}

void them_lenh() //Hien thi menu them lenh
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LENH SO: ");
  lcd.setCursor(9, 0);
  lcd.print(tong_lenh);  

  lcd.setCursor(3, 1);
  lcd.print("NHAP VI TRI: ");
  if(vitri < 10)
  {
    lcd.setCursor(16, 1);
    lcd.print(vitri);
    lcd.setCursor(17, 1);
    lcd.print("   ");
  }
  else if(vitri < 100)
  {
    lcd.setCursor(16, 1);
    lcd.print(vitri);
    lcd.setCursor(18, 1);
    lcd.print("  ");    
  }
  else if(vitri < 1000)
  {
    lcd.setCursor(16, 1);
    lcd.print(vitri);
    lcd.setCursor(19, 1);
    lcd.print(" ");    
  }

}

void nhap_time() //time delay cac VI TRI
{
      floattoint = (delay_vitri*5000)/5;
      time_vitri = map(floattoint, 0, 5000, 0, 255);     
      gui[15] = time_vitri; //gui 15 la gui delay giua cac VI TRI   
      gui[12] = vitri;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LENH SO: ");
  lcd.setCursor(9, 0);
  lcd.print(tong_lenh);  

  lcd.setCursor(6, 1);
  lcd.print("VI TRI: ");
  if(vitri < 10)
  {
    lcd.setCursor(14, 1);
    lcd.print(vitri);
    lcd.setCursor(15, 1);
    lcd.print("   ");
  }
  else if(vitri < 100)
  {
    lcd.setCursor(14, 1);
    lcd.print(vitri);
    lcd.setCursor(16, 1);
    lcd.print("  ");    
  }
  else if(vitri < 1000)
  {
    lcd.setCursor(14, 1);
    lcd.print(vitri);
    lcd.setCursor(17, 1);
    lcd.print(" ");    
  }

  lcd.setCursor(0, 2);
  lcd.print("Time Delay: ");  
  lcd.setCursor(12, 2);
  lcd.print(delay_vitri);    
  lcd.setCursor(16, 2);
  lcd.print("s");   
}

void time_cuoi() //time delay giua cac LENH
{
      floattoint = (delay_lenh*10000)/10;
      time_lenh = map(floattoint, 0, 10000, 0, 255);
      gui[16] = time_lenh; //Gui 16 la gui Delay giua cac LENH
      gui[10] = tong_lenh;    
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LENH SO: ");
  lcd.setCursor(9, 0);
  lcd.print(tong_lenh);  

  lcd.setCursor(0, 1);
  lcd.print("Thoi Gian Dung Giua");
  lcd.setCursor(6, 2);
  lcd.print("Cac Lenh");    
  lcd.setCursor(7, 3);
  lcd.print(delay_lenh);    
  lcd.setCursor(12, 3);
  lcd.print("s"); 
}

void TB_luulenh_moi()
{
  gui[11] = 4;
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("DANG LUU LENH MOI");  
  lcd.setCursor(0, 1);
  lcd.print(" LENH SO: ");
  lcd.setCursor(10, 1);
  lcd.print(tong_lenh);
  lcd.setCursor(0, 2);
  lcd.print(" SO VI TRI: ");
  lcd.setCursor(12, 2);
  lcd.print(vitri);
}

void tb_tonglenh() //hien thong bao khong the nhap them lenh
{
  lcd.clear();
  lcd.setCursor(4, 1);
  lcd.print("BO NHO DA DAY");  
  lcd.setCursor(0, 2);
  lcd.print("KHONG THE THEM LENH!");    
}

void ht_xoalenh() //hien thi MENU XOA LENH
{
  if(congtru_xoa == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">BACK"); 
    lcd.setCursor(0, 1);
    lcd.print(" XOA LENH BAT KY");
    lcd.setCursor(0, 2);
    lcd.print(" XOA TAT CA");  
  }
  else if(congtru_xoa == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" BACK"); 
    lcd.setCursor(0, 1);
    lcd.print(">XOA LENH BAT KY");
    lcd.setCursor(0, 2);
    lcd.print(" XOA TAT CA");     
  }
  else if(congtru_xoa == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" BACK"); 
    lcd.setCursor(0, 1);
    lcd.print(" XOA LENH BAT KY");
    lcd.setCursor(0, 2);
    lcd.print(">XOA TAT CA");    
  }
  
}
void xoa_lenh_bat_ky() //chon menu xoa lenh bat ky
{
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("CHON LENH CAN XOA");
  lcd.setCursor(2, 2);
  lcd.print("LENH SO: ");
  lcd.setCursor(11, 2);
  lcd.print(lenhxoa);     
}

void chon_lenh_DK() //hien thi menu chon lenh dieu khien
{
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("CHON LENH");    
  lcd.setCursor(5, 1);
  lcd.print("DIEU KHIEN");
  lcd.setCursor(0, 2);
  lcd.print(" LENH SO: ");  
  lcd.setCursor(10, 2);
  lcd.print(chon_lenh);    
  gui[17] = chon_lenh;
}
void so_lan_chay_lenh() //hien thi menu chon so lan thuc hien lenh
{
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("CHON SO LAN");
  lcd.setCursor(3, 1);
  lcd.print("THUC HIEN LENH");
  lcd.setCursor(0, 2);
  lcd.print(" SO LAN: "); 
  if(solan_chay == 0)
  {
    lcd.setCursor(9, 2);
    lcd.print("ALWAYS");
  } 
  else 
  {
    lcd.setCursor(9, 2);
    lcd.print(solan_chay);
  }

  gui[18] = solan_chay;
}

void TB_gui_lenh()
{
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("DA CHON LENH");
  lcd.setCursor(5, 1);
  lcd.print("DIEU KHIEN");

  lcd.setCursor(0, 2);
  lcd.print(" LENH SO: ");
  lcd.setCursor(10, 2);
  lcd.print(chon_lenh);

  lcd.setCursor(0, 3);
  lcd.print(" SO LAN: ");
  if(solan_chay == 0)
  {
    lcd.setCursor(9, 3);
    lcd.print("ALWAYS");
  } 
  else 
  {
    lcd.setCursor(9, 3);
    lcd.print(solan_chay);
  }    
}

void setup() 
{
  Serial.begin(9600);
  kiemtra = true;
  control = 1;

  lcd.init();lcd.backlight();  

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

  radio.openWritingPipe(diachi[0]);
  radio.openReadingPipe(1, diachi[1]);
  radio.setPALevel(RF24_PA_MIN); //Cài bộ khuyết địa công suất ở mức MIN
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);   

  if (!radio.available()) //khong ket noi
  {
    Serial.println("Chưa kết nối được với RX...!!");
    Serial.println("CHỜ KẾT NỐI.......");
  }   

  manhinh();
}

void loop()
{
  b = digitalRead(B); //Nut menu
  d = digitalRead(D); //Nut ON/OFF Control
  JOY = digitalRead(joy); //kep
  a = digitalRead(A);  //len menu
  c = digitalRead(C);  //xuong menu
  e = digitalRead(E); 
  gocX = analogRead(X);
    x = map(gocX, 0, 1023, 0, 100);
  gocY = analogRead(Y);
    y = map(gocY, 0, 1023, 0, 100); 

if(d != macdinh) //Nut ON/OFF Control
{
  if(d == 0)
  {
    if(demmenu == 0 && congtru_chinh == 0) //nut D o man hinh chinh
    {
      control += 1;
      if(control > 1) control = 0;
    }
    else if(demmenu == 3 && congtru_chinh == 3) //nhan D de vao Set delay LENH
    { 
      gui[11] = 3;
      demmenu = 5;
      time_cuoi();
    }
    else if(demmenu == 2 && congtru_chinh == 3) //tu menu them lenh thoat ra
    {
      demmenu = 1; congtru_chinh = 0;
      menu_chinh();  
      vitri = 0;
    }
    else if(demmenu == 2 && congtru_chinh == 2) //tu menu dieu khien ve menu chinh
    {
      gui[14] = 0;
      chon_lenh = 1;
      gui[17] = 1;
      demmenu = 1; congtru_chinh = 0;
      menu_chinh();
    }
    else if(demmenu == 3 && congtru_chinh == 4 && congtru_xoa == 1)
    {
      lenhxoa = 1;
      demmenu = 2; congtru_chinh = 4; congtru_xoa = 0;
      ht_xoalenh();
    }
  }
  delay(50);
  macdinh = d;
}   
ht_control();

if(a != macdinh) //len menu -
{
  if(a == 0)
  {
    if(demmenu == 1) //Menu tong
    {
      if(congtru_chinh <= 0) congtru_chinh = 4;
      else  congtru_chinh --;
      menu_chinh();
    }
    else if(demmenu == 3 && congtru_chinh == 3) //time delay tung vi tri
    {
      if(delay_vitri >= 5)  delay_vitri = 0.1;
      else  delay_vitri += 0.1;
      nhap_time();
    }
    else if(demmenu == 5 && congtru_chinh == 3) //man hinh Delay CUOI
    {
      if(delay_lenh >= 10)  delay_lenh = 0.1;
      else  delay_lenh += 0.1;
      time_cuoi();        
    }
    else if(demmenu == 2 && congtru_chinh == 2) //+ menu chon lenh dieu khien
    {
      if(chon_lenh >= nhan[4])  chon_lenh = 1;
      else chon_lenh ++;
      chon_lenh_DK();
    }
    else if(demmenu == 3 && congtru_chinh == 2) //+ So lan thuc hien
    {
      if(solan_chay >= 200)  solan_chay = 0;
      else solan_chay ++;
      so_lan_chay_lenh();      
    }
    else if(demmenu == 2 && congtru_chinh == 4) // - Menu xoa
    {
      if(congtru_xoa <= 0) congtru_xoa = 2;
      else  congtru_xoa --;
      ht_xoalenh();      
    }
    else if(demmenu == 3 && congtru_chinh == 4 && congtru_xoa == 1) //+ CHON LENH XOA
    {
      if(lenhxoa >= nhan[4])  lenhxoa = 1;
      else lenhxoa ++;      
      xoa_lenh_bat_ky();
    }
  }
  delay(50);
  macdinh = a;
}
if(c != macdinh) //xuong menu +
{
  if(c == 0)
  {
    if(demmenu == 1) //Menu tong
    {
      if(congtru_chinh >= 4) congtru_chinh = 0;
      else  congtru_chinh ++;
      menu_chinh();
    }   
    else if(demmenu == 3 && congtru_chinh == 3) //time delay tung vi tri
    {
      if(delay_vitri <= 0.1) delay_vitri = 5;
      else  delay_vitri -= 0.1;
      nhap_time();
    }
    else if(demmenu == 5 && congtru_chinh == 3) //man hinh Delay CUOI
    {
      if(delay_lenh <= 0.1) delay_lenh = 10;
      else  delay_lenh -= 0.1;
      time_cuoi();
    }
    else if(demmenu == 2 && congtru_chinh == 2) //- menu chon lenh dieu khien     
    {
      if(chon_lenh <= 1)  chon_lenh = nhan[4];
      else chon_lenh --;
      chon_lenh_DK();
    }  
    else if(demmenu == 3 && congtru_chinh == 2) //- So lan thuc hien
    {
      if(solan_chay <= 0)  solan_chay = 200;
      else solan_chay --;
      so_lan_chay_lenh();
    }
    else if(demmenu == 2 && congtru_chinh == 4) // + Menu xoa
    {
      if(congtru_xoa >=2) congtru_xoa = 0;
      else  congtru_xoa ++;
      ht_xoalenh();
    }
    else if(demmenu == 3 && congtru_chinh == 4 && congtru_xoa == 1) //- CHON LENH XOA
    {
      if(lenhxoa <= 1)  lenhxoa = nhan[4];
      else lenhxoa --;      
      xoa_lenh_bat_ky();
    }    
  }
  delay(50);
  macdinh = c;
}

if(b != macdinh && kiemtra == true) //Nut menu
{
  if(b == 0)
  {
    demmenu++;
    if(demmenu == 1 && gui[14] == 0) //Vao menu chinh
    {     
      menu_chinh();
    }
    else if(demmenu == 2 && congtru_chinh == 0) //Nhan Back tu Menu tong ve man hinh
    {
      manhinh();
      demmenu = 0;
    }
    else if(demmenu == 2 && congtru_chinh == 1) //gui tin hieu AUTO HOME
    {
      gui[8] = 1; nhan[8] = 1;
      demmenu = 1;

      if(gui[9] == 0) //dang ON
      {
        gui[9] = 1; //OFF
        control = 1;
      }       
      else  //1 Dang off
      {
        gui[9] = 1;
        control = 1;
      }
    }  
    
    else if(demmenu == 2 && congtru_chinh == 3) //Vao menu them lenh
    {
      control = 0; //DIEU KHIEN ON
      gui[9] = 0;
      gui[15] = 0; gui[16] = 0;      

      vitri += 1; //Tang vi tri them 1
      if(nhan[4] >= 200) //neu so lenh lon hon 200
      {
        tb_tonglenh();
        delay(5000);
        demmenu = 1;
        menu_chinh();
      }
      else 
      {
        tong_lenh = nhan[4] + 1;
        them_lenh();
      }
    } 
    else if(demmenu == 3 && congtru_chinh == 3) //luu vi tri
    {
      control = 1; //DIEU KHIEN OFF
      gui[11] = 1;
      nhap_time(); //delay vi tri   
    }
    else if(demmenu == 4 && congtru_chinh == 3) //luu delay vi tri
    {
      control = 1; //DIEU KHIEN OFF
      demmenu = 2;
      gui[11] = 2;
      vitri += 1; //Tang vi tri them 1
      them_lenh();     
    }
    else if(demmenu == 6 && congtru_chinh == 3) //luu delay lenh va ve menu chinh
    {
      control = 1; //DIEU KHIEN OFF
      gui[11] = 4;
      TB_luulenh_moi();
      time1 = millis();
      kiemtra = false;
    }
    
      else if(demmenu == 2 && congtru_chinh == 2) // Vao menu lenh dieu khien
      {
        control = 1; //DIEU KHIEN OFF
        if(nhan[4] > 0) //co lenh
        {
          gui[14] = 1;
          chon_lenh_DK();
        }
        else  //khong co lenh nao duoc luu
        {
          lcd.clear();
          lcd.setCursor(3, 1);
          lcd.print("HIEN KHONG CO");    
          lcd.setCursor(1, 2);    
          lcd.print("LENH NAO DUOC LUU");
          delay(3000);
          demmenu = 1; congtru_chinh = 0;
          menu_chinh();           
        }        
      }
      else if(demmenu == 3 && congtru_chinh == 2) //Chon so lan thuc hien: 1-200 or ALWAYS
      {
        gui[14] = 2;
        control = 1; //DIEU KHIEN OFF
        so_lan_chay_lenh();
      }
      else if(demmenu == 4 && congtru_chinh == 2) //gui lenh thuc hien va thoat ve Menu
      {
        control = 1; //DIEU KHIEN OFF
        TB_gui_lenh();        
      }
      else if(demmenu == 5 && congtru_chinh == 2) //tro ve man hinh
      {
        gui[19] = 0;
        gui[14] = 3;
        demmenu = 0; congtru_chinh = 0;
        manhinh();
      }

      else if(demmenu == 1 && gui[14] == 3 && congtru_chinh == 0) //gui lenh STOP
      {
        gui[19] = 1; //thuc hien STOP
        time1 = millis();
      }

    else if(demmenu == 2 && congtru_chinh == 4) // Vao menu Xoa LENH
    {
      control = 1; //DIEU KHIEN OFF
      if(nhan[4] > 0)
      {
        gui[13] = 0;
        ht_xoalenh();
      }
      else
      {
        lcd.clear();
        lcd.setCursor(3, 1);
        lcd.print("HIEN KHONG CO");    
        lcd.setCursor(1, 2);    
        lcd.print("LENH NAO DUOC LUU");
        delay(3000);
        demmenu = 1; congtru_chinh = 0;
        menu_chinh();         
      }
    }
    else if(demmenu == 3 && congtru_chinh == 4 && congtru_xoa == 0)//nhan BACK o MENU XOA
    {
      control = 1; //DIEU KHIEN OFF
      demmenu = 1; congtru_chinh = 0;
      menu_chinh();       
    }
    else if(demmenu == 3 && congtru_chinh == 4 && congtru_xoa == 1) //nhan vao XOA LENH BAT KY
    {
      control = 1; //DIEU KHIEN OFF
      xoa_lenh_bat_ky();
    }
    else if(demmenu == 4 && congtru_chinh == 4 && congtru_xoa == 1) //Gui lenh xoa
    {
      control = 1; //DIEU KHIEN OFF
      tong_lenh = 0; gui[10] = 0;
      gui[13] = 1;
      gui[20] = lenhxoa;
      
      lcd.clear();
      lcd.setCursor(5, 1);
      lcd.print("DANG XOA...");    
      lcd.setCursor(2, 2);    
      lcd.print("LENH SO: ");
      lcd.setCursor(11, 2);    
      lcd.print(lenhxoa);      
    }
    else if(demmenu == 3 && congtru_chinh == 4 && congtru_xoa == 2) //nhan vao XOA TAT CA
    {
      control = 1; //DIEU KHIEN OFF
      gui[13] = 2; //gui lenh XOA TAT CA
      tong_lenh=0;
      gui[10] = 0;
      lcd.clear();
      lcd.setCursor(6, 1);
      lcd.print("DANG XOA");    
      lcd.setCursor(3, 2);    
      lcd.print("TAT CA CAC LENH");     
      time1=millis();
    }
  }
  delay(50);
  macdinh = b;
}

if(nhan[13] == 1)//sau khi thuc hien XOA xong
{
  gui[20] = 0;
  demmenu = 0; congtru_chinh = 0; congtru_xoa = 0;
  manhinh();  
  lenhxoa = 1;
  gui[13] = 0;
}

if((millis() > time1 + 3000) && gui[13] == 2 && demmenu == 3 && congtru_chinh == 4 && congtru_xoa == 2) //ve man hinh sau khi XOA TAT CA
{
  gui[13] = 0;
  demmenu = 0; congtru_chinh = 0; congtru_xoa = 0;
  manhinh();
}

if((millis() > time1 + 3000) && gui[14] == 3 && demmenu == 1 && congtru_chinh == 0) //sau 3s sau khi nhan STOP
{
  gui[19] = 0; gui[14] = 0;
  demmenu = 0;
  chon_lenh = 1; solan_chay = 0;
  manhinh();
}

if(gui[14] == 3)// Neu dang chay lenh
{
  control = 1; //DIEU KHIEN OFF
}

if(nhan[14] == 1) //sau khi thuc hien lenh xong
{
  gui[14] = 0;
  chon_lenh = 1; solan_chay = 0;
  manhinh();
}

if((millis() > time1 + 3000) && (demmenu == 6 && congtru_chinh == 3)) //luu delay LENH, ve MENU
{
  time1 = millis();
  demmenu = 1; congtru_chinh = 0;
  menu_chinh();  
  vitri = 0;  
  kiemtra = true;
}

if(demmenu == 1)
{
  gui[11] = 0; //kiem tra luu
  gui[15] = 0; //delay vi tri
  gui[16] = 0; //delay lenh
}

  radio.stopListening(); //Ngưng nhận
  if(demmenu == 0 or demmenu == 2) //O Menu tong moi gui
  {
    if(congtru_chinh == 0 or congtru_chinh == 3)
    {
      gui[0] = a; 
      gui[2] = c;
    }
  }
  gui[1] = b;
  gui[3] = d;
  gui[4] = e; 
  gui[5] = JOY;
  gui[6] = x;
  gui[7] = y; 
  radio.write(&gui, sizeof(gui)); //Gui du lieu  
  delay(10);

  radio.startListening(); //Bắt đầu nhận
  while(!radio.available());
  radio.read(&nhan, sizeof(nhan)); //nhan 
  delay(10);

  if(nhan[8] == 0)  gui[8] = 0;

  ht_goc(); //hien thi goc len man hinh chinh

  // Serial.print("  STOP: "); Serial.print(gui[19]);
  // Serial.print("  LENH: "); Serial.print(gui[17]);
  // Serial.print("  SL: "); Serial.print(gui[18]);
  // Serial.print(" 14 DK: "); Serial.print(gui[14]);
  // Serial.print(" 12 XOA: "); Serial.print(gui[13]);
  // Serial.print(" Tong LENH: "); Serial.print(nhan[4]);
  // Serial.print("Delay lenh: "); Serial.print(delay_lenh);
  // Serial.print("  Vi tri: "); Serial.print(vitri);
  // Serial.print("Delay vi tri: "); Serial.print(delay_vitri);
  // Serial.print("  Floattoint: "); Serial.print(floattoint);
  // Serial.print("  Delay gui: "); Serial.print(delay_gui);
  // Serial.print(" MILLIS: "); Serial.print(millis());
  // Serial.print(" TIME1: "); Serial.print(time1);
  // Serial.print(" Tong LENH: "); Serial.print(gui[10]);
  // Serial.print(" GUI 11: "); Serial.print(gui[11]);
  // Serial.print(" 12_VT: "); Serial.print(gui[12]);
  // Serial.print("  delay VT: "); Serial.print(gui[15]);
  // Serial.print("  delay Lenh: "); Serial.print(gui[16]);
  //Serial.print("nut control: "); Serial.print(control); 

  // Serial.print(" A: "); Serial.print(gui[0]);
  // Serial.print(" B: "); Serial.print(gui[1]);
  // Serial.print(" C: "); Serial.print(gui[2]);
  // Serial.print(" D: "); Serial.print(gui[3]);
  // Serial.print(" E: "); Serial.print(gui[4]);
  // Serial.print(" JOY: "); Serial.print(gui[5]);
  // Serial.print(" X: "); Serial.print(gui[6]);
  // Serial.print(" Y: "); Serial.print(gui[7]);  

// Serial.print(" TongLenh: "); Serial.print(tong_lenh);
// Serial.print(" nhan_4: "); Serial.print(nhan[4]);
// Serial.print(" GUI_10: "); Serial.print(gui[10]);
//   Serial.print(" 13 XOA: "); Serial.print(gui[13]);
//   Serial.print(" 20 LENH XOA: "); Serial.print(gui[20]);
//   Serial.print("  +/- XOA: "); Serial.print(congtru_xoa);    
  
  // Serial.print("  Kiem tra: "); Serial.print(kiemtra);
  // Serial.print("  - Dem menu: "); Serial.print(demmenu);  
  // Serial.print("  +/- Chinh: "); Serial.print(congtru_chinh);

  // Serial.println();

}

  /*
  * GUI:
  * 0-A; 1-B; 2-C; 3-D; 4-E; 5-JOY; 6-X; 7-Y
  * 8-Auto home; 9-Control
  * 10- Tong so lenh;  11- kiem tra luu
  * 
  * 14- lenh DIEU KHIEN
  * 17- Lenh DK; 18- So lan thuc hien
  * 
  * 19- Lenh STOP
  * 
  * 13- Lenh xoa; 20- lenh CAN XOA
  * 
  * 12- gui vi tri; 
  * 15-Delay Vi Tri; 16-Delay Cuoi
  * 
  * NHAN: 
  * 0-A; 1-B; 2-C; 3-KEP; 8-Reset auto home
  * 4-doc eeprom 0/TONG LENH
  * 
  * 14- reset lenh DIEU KHIEN
  * 13- sau khi XOA xong
  */