#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Eeprom24Cxx.h>
static Eeprom24C eeprom(256,0x50);

Servo servo_z;
Servo servo_a;
Servo servo_b;
Servo servo_kep;

#define Servo_kep 8   //Servo kep
#define Servo_Z   7 //Servo truc Z
#define Servo_A   6 //Servo truc A - truc ngan
#define Servo_B   5 //Servo truc B - truc dai

RF24 radio(10, 9); // CE, CSN
const byte diachi[][6] = {"12345", "10000"}; //0, 1
// const byte diachi[6] = "12345";
byte nhan[30];
byte gui[30];
unsigned long dem_kep = 0; byte macdinh = 1;

byte tonglenh;

void dk_kep() //Dieu khien kep
{
  if(nhan[5] != macdinh)
  {
    if(nhan[5] == 0)
    {dem_kep+=1;}
    macdinh = nhan[5];
  }
  //Serial.print(dem); Serial.print(" --- ");
  if( (dem_kep%2) == 1 ) 
  {
    servo_kep.write(0); //MO
    gui[3] = 0;
    //Serial.print("MO  --- ");
  }
  else 
  {
    servo_kep.write(180); //KEP
    gui[3] = 1;
    //Serial.print("KEP  --- ");
  }
}

void truc_z() //Dieu khien truc Z
{
  byte goc_z; byte goc_moi; byte cong; byte tru;
  if(nhan[6] >= 50 && nhan[6] <= 100) //phai
  {
    goc_z = servo_z.read();
    if(goc_z < 180)
    {
      cong = map(nhan[6], 50, 100, 0, 1);
      goc_z = goc_z + cong;
      servo_z.write(goc_z);
      tru = 0;
    }
    else 
    {
      cong = 0; tru = 0;
      servo_z.write(180);
    }
  }
  //if(nhan[6] >= 0 && nhan[6] <= 49) //trai
  else
  {
    goc_z = servo_z.read();
    if(goc_z > 0)
    {
      tru = map(nhan[6], 49, 0, 0, 1);
      goc_z = goc_z - tru;
      servo_z.write(goc_z);
      cong = 0;
    }
    else  
    {
      tru = 0; cong = 0;
      servo_z.write(0);   
    }
  }
  // Serial.print("cong: "); Serial.print(cong);
  // Serial.print("  tru: "); Serial.print(tru);
  // Serial.print("  Z: "); Serial.print(goc_z); 
}

void truc_a() //Dieu khien truc A
{
  byte goc_a; byte goc_moi; byte cong; byte tru;
  if(nhan[7] >= 50 && nhan[7] <= 100) //Len .... 90
  {
    goc_a = servo_a.read();
    if(goc_a < 90)
    {
      cong = map(nhan[7], 50, 100, 0, 1);
      goc_a = goc_a + cong;
      servo_a.write(goc_a);
      tru = 0;
    }
    else 
    {
      cong = 0; tru = 0;
      servo_a.write(90);
    }
  }
  //if(nhan[7] >= 0 && nhan[7] <= 49) //xuong
  else // xuong ...... 50
  {
    goc_a = servo_a.read();
    if(goc_a > 50)
    {
      tru = map(nhan[7], 49, 0, 0, 1);
      goc_a = goc_a - tru;
      servo_a.write(goc_a);
      cong = 0;
    }
    else  
    {
      tru = 0; cong = 0;
      servo_a.write(50);   
    }
  }
  // Serial.print("cong: "); Serial.print(cong);
  // Serial.print("  tru: "); Serial.print(tru);
  // Serial.print("  A: "); Serial.print(goc_a); 
}

void truc_b() //Dieu khien truc B
{
  byte gocb;
  gocb = servo_b.read();
  if(nhan[0] == 0) //nut A - len ..... 135
  {
    if(gocb < 115)
    {
      gocb = gocb + 1;
      servo_b.write(gocb);
    }
    else  servo_b.write(115);  
  }
  if(nhan[2] == 0) //nut B - xuong ..... 50
  {
    if(gocb > 50)
    {
      gocb = gocb - 1;
      servo_b.write(gocb);
    }
    else  servo_b.write(50);   
  }  

  // Serial.print("cong: "); Serial.print(cong);
  // Serial.print("  tru: "); Serial.print(tru);
  //Serial.print("B: "); Serial.print(gocb);   
}

void doc_servo() //Doc cac goc hien tai cua Servo
{
  gui[0] = servo_a.read(); //Truc A
  gui[1] = servo_b.read(); //Truc B
  gui[2] = servo_z.read(); //Truc Z
}

void tong_lenh() //doc, ghi tong lenh
{
  tonglenh = eeprom.read_1_byte(0);
  gui[4] = tonglenh;
  if(nhan[10] > tonglenh)
  {
    eeprom.write_1_byte(0, nhan[10]); 
  }
  //Serial.print(" Tong lenh: "); Serial.print(gui[4]);
}

void in_eeprom()
{
  Serial.println();

  // Serial.print(" gui 11: "); Serial.print(gui[11]);
  // Serial.print(" nhan 15: "); Serial.print(nhan[15]); //delay vi tri
  // Serial.print(" nhan 16: "); Serial.print(nhan[16]); //delay lenh

  Serial.print("  Tong lenh: "); Serial.print(eeprom.read_1_byte(0));

  Serial.print("  - Cuoi VT: "); Serial.print(eeprom.read_2_byte(4));
  Serial.print("  - Cuoi lenh: "); Serial.println(eeprom.read_2_byte(6));

Serial.print("VUNG VI TRI: ");
  Serial.print(" 11: "); Serial.print(eeprom.read_1_byte(11));
  Serial.print(" 12: "); Serial.print(eeprom.read_2_byte(12));
  Serial.print(" 14: "); Serial.print(eeprom.read_2_byte(14));

  Serial.print(" - 16: "); Serial.print(eeprom.read_1_byte(16));
  Serial.print(" 17: "); Serial.print(eeprom.read_2_byte(17));
  Serial.print(" 19: "); Serial.print(eeprom.read_2_byte(19));  

  Serial.print(" - 21: "); Serial.print(eeprom.read_1_byte(21));
  Serial.print(" 22: "); Serial.print(eeprom.read_2_byte(22));
  Serial.print(" 24: "); Serial.print(eeprom.read_2_byte(24));

  Serial.print(" - 26: "); Serial.print(eeprom.read_1_byte(26));
  Serial.print(" 27: "); Serial.print(eeprom.read_2_byte(27));
  Serial.print(" 29: "); Serial.println(eeprom.read_2_byte(29));      

Serial.print("VUNG LENH: ");
  for(int i = 999; i<=1040; i++) //in tu 998 - 1020
  {
    Serial.print(i); Serial.print(": "); Serial.print(eeprom.read_1_byte(i));  
    Serial.print("  ");
  }

  Serial.println();
  delay(100);

}

  unsigned int diachi_vitri;//doc o nho lenh cuoi
  unsigned int diachi_lenh;
  unsigned int diachi_a;
  unsigned int diachi_b;
  unsigned int diachi_c;
  unsigned int diachi_kep;  

  unsigned int diachi_vitri_cuoi; //doc o nho vi tri cuoi
  unsigned int diachi_lenh_cuoi;
  unsigned int diachi_STT_lenh;

  unsigned long diachi_batdau;
  unsigned long diachi_ketthuc;
  unsigned int so_vitri;

void luu_lenh_moi()
{
  if(nhan[11] == 0)
  {
  
  }
  else if(nhan[11] == 1) //luu goc va delay VI TRI
  {
    //diachi_STT_lenh = eeprom.read_2_byte(4); //doc dia chi VI TRI cuoi cung
    diachi_vitri = eeprom.read_2_byte(6);
    diachi_a = diachi_vitri + 1;
    diachi_b = diachi_vitri + 2;
    diachi_c = diachi_vitri + 3;
    diachi_kep = diachi_vitri + 4;
    diachi_vitri += 5;
    eeprom.write_1_byte(diachi_vitri, nhan[15]); //luu delay vi tri
    eeprom.write_1_byte(diachi_a, gui[0]); //luu goc A
    eeprom.write_1_byte(diachi_b, gui[1]); //luu goc B
    eeprom.write_1_byte(diachi_c, gui[2]); //luu goc C
    if(gui[3] == 0) //MO
      eeprom.write_1_byte(diachi_kep, 0); //luu goc KEP
    else 
      eeprom.write_1_byte(diachi_kep, 180); //luu goc KEP  
    
    so_vitri = nhan[12];

  }
  else if(nhan[11] == 2) //ghi dia chi LENH cuoi MOI
  { 
    eeprom.write_2_byte(6, diachi_vitri);
    //in_eeprom();
  }
  else if(nhan[11] == 3) //luu delay lenh va TONG LENH
  {
    diachi_STT_lenh = eeprom.read_2_byte(4); //doc dia chi VI TRI cuoi cung
    diachi_STT_lenh += 1;
    diachi_lenh = diachi_vitri+1;
    eeprom.write_1_byte(diachi_lenh, nhan[16]); //luu delay lenh
    eeprom.write_2_byte(6, diachi_lenh); //luu dia chi lenh cuoi cung

    eeprom.write_1_byte(diachi_STT_lenh, nhan[10]); //ghi STT LENH
    
    diachi_batdau = diachi_lenh - (5*so_vitri);
    eeprom.write_2_byte(diachi_STT_lenh + 1, diachi_batdau);

    //dia chi lenh chinh la dia chi ket thuc
    diachi_ketthuc = diachi_STT_lenh + 3;
    eeprom.write_2_byte(diachi_ketthuc, diachi_lenh);
     
  }
  else if(nhan[11] == 4) //ghi dia chi vi tri cuoi MOI
  {
    // luu o cuoi cung cua vi tri
    diachi_vitri_cuoi = diachi_ketthuc + 1;
    eeprom.write_2_byte(4, diachi_vitri_cuoi);
    //in_eeprom();
  }

// Serial.print("  Nhan 10: "); Serial.print(nhan[10]); //Tong lenh
// Serial.print("  Nhan 11: "); Serial.print(nhan[11]); //kiem tra luu
// Serial.print("  12_VT: "); Serial.print(nhan[12]); //so vi tri
// Serial.print("  DC STT: "); Serial.print(diachi_STT_lenh); //dia chi luu stt
// Serial.print(" - "); Serial.print(eeprom.read_1_byte(11)); //STT lenh
// Serial.print("  DC BD: "); Serial.print(diachi_STT_lenh + 1); //dia chi bat dau
// Serial.print(" - "); Serial.print(eeprom.read_2_byte(diachi_STT_lenh + 1));
// Serial.print("  DC KT: "); Serial.print(diachi_ketthuc); //dia chi ket thuc
// Serial.print(" - "); Serial.print(eeprom.read_2_byte(diachi_ketthuc));
// Serial.print("  DC VT Cuoi: "); Serial.print(diachi_vitri_cuoi);
// Serial.print(" - "); Serial.print(eeprom.read_2_byte(4));
// Serial.print("  Nhan 15: "); Serial.print(nhan[15]);
// Serial.print("  Nhan 16: "); Serial.print(nhan[16]);
// Serial.print("  a: "); Serial.print(diachi_a);
// Serial.print("  b: "); Serial.print(diachi_b);
// Serial.print("  c: "); Serial.print(diachi_c);
// Serial.print("  kep: "); Serial.print(diachi_kep);
// Serial.print("  DC VT: "); Serial.print(diachi_vitri);
// Serial.print("  DC 6: "); Serial.print(eeprom.read_2_byte(6));
// Serial.print("  DC LENH: "); Serial.println(diachi_lenh);

}

unsigned int lenhxoa;
unsigned int o_batdau;
unsigned int o_ketthuc;
unsigned int vitri_cuoi;
unsigned int lenh_cuoi;
unsigned int tong_vitri;
unsigned int diachi_moi;
unsigned int diachi_o_lenh;
unsigned int diachi_lenhxoa;
unsigned int a;
byte doc_o_vitri;
unsigned int doc_batdau;
unsigned int doc_ketthuc;
byte giatri;

void xoa_lenh()
{
  if(nhan[13] == 1)//XOA LENH BAT KY
  {
    vitri_cuoi = eeprom.read_2_byte(4);
    lenh_cuoi = eeprom.read_2_byte(6);
    tonglenh = eeprom.read_1_byte(0);

    if(eeprom.read_1_byte(0) == 1) //Neu chi co 1 lenh
    {
      for(int i=10; i<=vitri_cuoi; i++)
      {
        lenhxoa = eeprom.read_1_byte(i);
        if(lenhxoa == nhan[20])
        {
          o_batdau = eeprom.read_2_byte(i+1);
          o_ketthuc = eeprom.read_2_byte(i+3);
          eeprom.write_1_byte(i, 0);
          eeprom.write_2_byte(i+1, 0);
          eeprom.write_2_byte(i+3, 0);

          for(int i=o_batdau; i<=o_ketthuc; i++)
          {
            eeprom.write_1_byte(i, 0);
          }
          eeprom.write_2_byte(4, 10);
          eeprom.write_2_byte(6, 999);
          eeprom.write_1_byte(0, 0);
          // in_eeprom();
          // Serial.print(" Bat dau: "); Serial.print(o_batdau);
          // Serial.print(" ket thuc: "); Serial.println(o_ketthuc); 
          break;
        }
      }
      gui[13] = 1;//gui thong bao da xoa xong
    }
    else //CO NHIEU HON 1 LENH
    {
      for(int i=11; i<=vitri_cuoi; i+=5)
      {
        lenhxoa = eeprom.read_1_byte(i); //lenhxoa = gia tri epprom
        if(lenhxoa == nhan[20])
        {
          diachi_lenhxoa = i;
          o_batdau = eeprom.read_2_byte(i+1); //doc o bat dau
          o_ketthuc = eeprom.read_2_byte(i+3); //doc o ket thuc
          // Serial.print(" DIACHI_lenhXOA: "); Serial.print(diachi_lenhxoa);
          // Serial.print(" batdau: "); Serial.print(o_batdau);
          // Serial.print(" Ketthuc: "); Serial.println(o_ketthuc);
          break;
        }
      }
 
      if((diachi_lenhxoa + 4) == vitri_cuoi) //neu lenh o CUOI
      {
        for(int j=o_batdau; j<=o_ketthuc; j++) //xoa tu BAT DAU - KET THUC
        {
          eeprom.write_1_byte(j, 0);
        }
        eeprom.write_1_byte(diachi_lenhxoa, 0); //STT lenh
        eeprom.write_2_byte(diachi_lenhxoa+1, 0); //dia chi bat dau
        eeprom.write_2_byte(diachi_lenhxoa+3, 0); //dia chi ket thuc
        
        eeprom.write_2_byte(4, diachi_lenhxoa-1); //cap nhat VI TRI CUOI
        eeprom.write_2_byte(6, o_batdau-1); //cap nhat LENH CuOI
        eeprom.write_1_byte(0, tonglenh-1); //cap nhat TONG LENH

        //in_eeprom();        
      }
      else  //Nguoc lai LENH o GIUA
      {
        for(int j=o_batdau; j<=o_ketthuc; j++)
        {
          eeprom.write_1_byte(j, 0);
        }
        eeprom.write_1_byte(diachi_lenhxoa, 0); //STT lenh
        eeprom.write_2_byte(diachi_lenhxoa+1, 0); //dia chi bat dau
        eeprom.write_2_byte(diachi_lenhxoa+3, 0); //dia chi ket thuc

        o_ketthuc += 1;
        tong_vitri = o_ketthuc - o_batdau;       
        for(o_ketthuc; o_ketthuc<=lenh_cuoi; o_ketthuc++) //chay tu o cuoi toi CUOI LENH
        {
          diachi_moi = o_ketthuc - tong_vitri; 
          giatri = eeprom.read_1_byte(o_ketthuc);
          eeprom.write_1_byte(diachi_moi, giatri);
          eeprom.write_1_byte(o_ketthuc, 0);
          eeprom.write_2_byte(6, diachi_moi);
          // Serial.print(" DC_moi: "); Serial.print(diachi_moi);
          // Serial.print(" batdau: "); Serial.print(o_batdau);
          // Serial.print(" Ketthuc: "); Serial.println(o_ketthuc);          
          // in_eeprom();
        }

        diachi_o_lenh = diachi_lenhxoa + 5;
            for(int g=diachi_o_lenh; g<=vitri_cuoi; g+=5) //cap nhat la Vi Tri
            {
              a=g;
              doc_o_vitri = eeprom.read_1_byte(a);
              doc_batdau = eeprom.read_2_byte(a+1);
              doc_ketthuc = eeprom.read_2_byte(a+3);

              eeprom.write_1_byte(a-5, doc_o_vitri - 1);
              eeprom.write_2_byte(a-4, doc_batdau - tong_vitri);
              eeprom.write_2_byte(a-2, doc_ketthuc - tong_vitri);

              eeprom.write_1_byte(a, 0);
              eeprom.write_2_byte(a+1, 0);
              eeprom.write_2_byte(a+3, 0); 

              eeprom.write_2_byte(4, a-1); //cap nhat VI TRI CUOI
              eeprom.write_1_byte(0, tonglenh-1); //cap nhat TONG LENH

              // Serial.print(" vitri: "); Serial.print(doc_o_vitri);
              // Serial.print(" Bat dau: "); Serial.print(doc_batdau);
              // Serial.print(" ketthuc: "); Serial.println(doc_ketthuc);
              // in_eeprom();
            }         
      }
      gui[13] = 1;
    }
  }
  else if(nhan[13] == 2) //nhan duoc LENH XOA TAT CA
  {
    vitri_cuoi = eeprom.read_2_byte(4);
    lenh_cuoi = eeprom.read_2_byte(6);

    for (int i = 9 ; i <= vitri_cuoi ; i++) 
    {
      eeprom.write_1_byte(i, 0);
    }
    for (int i = 990 ; i <= lenh_cuoi ; i++) 
    {
      eeprom.write_1_byte(i, 0);
    }  

    eeprom.write_1_byte(0, 0); //ghi vao o tong lenh
    eeprom.write_2_byte(4, 10);
    eeprom.write_2_byte(6, 999);    
    gui[4] = 0;
    gui[13] = 1; //gui thong bao da xoa xong
    //in_eeprom();
  }
  else if(nhan[13] == 0)
  {
    gui[13] = 0;
  }
}

  byte lenh;
  unsigned int diachi_cuoi_vitri;
  unsigned int diachi_cuoi_lenh;
  unsigned int batdau; unsigned int ketthuc;
  unsigned int solan;
  unsigned int vitri;
  unsigned int i=0; unsigned int j=0;
  unsigned int delay_vitri; unsigned int delay_lenh;
int JA; int JB; int JZ; int J_KEP; int dung_VT; int dung_lenh;
void chay_lenh()
{
  /*
  * 14- lenh DIEU KHIEN
  * 17- Lenh DK; 18- So lan thuc hien
  */
  if(nhan[14] == 0)
  {
    i=0; j=0;
    gui[14] = 0;
  }
  else if(nhan[14] == 1)
  {
    lenh = nhan[17]; //STT lenh
    //eeprom.write_1_byte(1, lenh); //ghi STT lenh vao o 1
    diachi_cuoi_vitri = eeprom.read_2_byte(4); //doc dia chi VI TRI cuoi cung
    for(int diachi = 10; diachi <= diachi_cuoi_vitri; diachi++) //chay tu 10 den o VT cuoi
    {
      if(eeprom.read_1_byte(diachi) == lenh) // dia chi nao co gia tri bang LENH
      {
        batdau = eeprom.read_2_byte(diachi + 1);
        ketthuc = eeprom.read_2_byte(diachi + 3);
      }
    }
  }
  else if(nhan[14] == 2) //luu so lan thuc hien lenh
  {
    solan = nhan[18]; //So lan thuc hien lenh
  }
  else if(nhan[14] == 3) //thuc hien lenh
  {
    vitri = (ketthuc - batdau)/5; 

    if(solan == 0) //chay lien tuc
    {
      JA = batdau;
          for(int j=0; j<vitri; j++)
          {
            if(nhan[19] == 1)
            {
              gui[14] = 1;
              break;
            }
            JB = JA + 1;
            JZ = JA + 2;
            J_KEP = JA + 3;
            //dung_VT = JA + 4;

            servo_b.write(eeprom.read_1_byte(JB));
            servo_z.write(eeprom.read_1_byte(JZ));
            servo_a.write(eeprom.read_1_byte(JA));
            servo_kep.write(eeprom.read_1_byte(J_KEP));
            dung_VT = eeprom.read_1_byte(JA+4);
            delay_vitri = map(dung_VT, 0, 255, 0, 5000);  

            // Serial.print(" NHAN_19: "); Serial.print(nhan[19]);
            // Serial.print(" - A: "); Serial.print(JA);
            // Serial.print(" B: "); Serial.print(JB);
            // Serial.print(" Z: "); Serial.print(JZ);
            // Serial.print(" KEP: "); Serial.print(J_KEP);
            // Serial.print(" Delay VT: "); Serial.println(dung_VT);      
            JA += 5; 

            delay(delay_vitri);     
          } 

      //dung_lenh = ketthuc;
      dung_lenh = eeprom.read_1_byte(ketthuc);
      delay_lenh = map(dung_lenh, 0, 255, 0, 10000);      
      // Serial.print(" Delay LENH: "); Serial.println(dung_lenh);   
      delay(delay_lenh);
    }
    else  // >0
    {
      if(i<solan)
      {
        if(vitri == 1)
        {
          JA = batdau;
          JB = batdau+1;
          JZ = batdau+2;
          J_KEP = batdau+3;
          //dung_VT = batdau+4;
          dung_VT = eeprom.read_1_byte(batdau+4);
          delay_vitri = map(dung_VT, 0, 255, 0, 5000);
          
          servo_b.write(eeprom.read_1_byte(JB));
          servo_z.write(eeprom.read_1_byte(JZ));
          servo_a.write(eeprom.read_1_byte(JA));
          servo_kep.write(eeprom.read_1_byte(J_KEP));

          // Serial.print(" i: "); Serial.print(i);
          // Serial.print(" - A: "); Serial.print(JA);
          // Serial.print(" B: "); Serial.print(JB);
          // Serial.print(" Z: "); Serial.print(JZ);
          // Serial.print(" KEP: "); Serial.print(J_KEP);
          // Serial.print(" Delay VT: "); Serial.print(delay_vitri);

          delay(delay_vitri);
        }
        else //Vi tri > 1
        {
          JA = batdau;
          for(int j=0; j<vitri; j++)
          {
            if(nhan[19] == 1)
            {
              gui[14] = 1;
              break;
            }            
            JB = JA + 1;
            JZ = JA + 2;
            J_KEP = JA + 3;
            //dung_VT = JA + 4;

            servo_b.write(eeprom.read_1_byte(JB));
            servo_z.write(eeprom.read_1_byte(JZ));
            servo_a.write(eeprom.read_1_byte(JA));
            servo_kep.write(eeprom.read_1_byte(J_KEP));
            dung_VT = eeprom.read_1_byte(JA+4);
            delay_vitri = map(dung_VT, 0, 255, 0, 5000);            

            //Serial.print(" j: "); Serial.print(j);
            // Serial.print(" NHAN_19: "); Serial.print(nhan[19]);
            // Serial.print(" - A: "); Serial.print(JA);
            // Serial.print(" B: "); Serial.print(JB);
            // Serial.print(" Z: "); Serial.print(JZ);
            // Serial.print(" KEP: "); Serial.print(J_KEP);
            // Serial.print(" Delay VT: "); Serial.println(dung_VT);      
            JA += 5;      

            delay(delay_vitri);
          }
        }

        //dung_lenh = ketthuc;
        dung_lenh = eeprom.read_1_byte(ketthuc);
        delay_lenh = map(dung_lenh, 0, 255, 0, 10000);
        // Serial.print(" Delay LENH: "); Serial.println(delay_lenh);
        delay(delay_lenh);

        i++;
      }
      else //i = solan HOAC i>solan la thuc hien XONG lenh
      {
        gui[14] = 1;
        // Serial.print(" i: "); Serial.println(i);
      } 
    }   
  }

  // Serial.print(" 14_chay: "); Serial.print(nhan[14]);  

  // Serial.print(" - Lenh: "); Serial.print(lenh);
  // Serial.print(" Cuoi VT: "); Serial.print(diachi_cuoi_vitri);
  // Serial.print(" BDau: "); Serial.print(batdau);
  // Serial.print(" KThuc: "); Serial.print(ketthuc);
  // Serial.print(" So VT: "); Serial.print(vitri);
  // Serial.print(" SL: "); Serial.println(solan);  
}

void setup() 
{
  Serial.begin(9600);
  // for (int i = 0 ; i < 100 ; i++) 
  // {
  //   eeprom.write_1_byte(i, 0);
  // }
  // for (int i = 900 ; i < 1200 ; i++) 
  // {
  //   eeprom.write_1_byte(i, 0);
  // }  

  // eeprom.write_2_byte(4, 10);
  // eeprom.write_2_byte(6, 999);

servo_kep.attach(Servo_kep);
servo_a.attach(Servo_A);
servo_b.attach(Servo_B);
servo_z.attach(Servo_Z);

	// Allow allocation of all timers
	// ESP32PWM::allocateTimer(0);
	// ESP32PWM::allocateTimer(1);
	// ESP32PWM::allocateTimer(2);
	// ESP32PWM::allocateTimer(3);

  // servo_a.setPeriodHertz(50);
  // servo_a.attach(Servo_A, 500, 2500);
  // servo_b.setPeriodHertz(50);
  // servo_b.attach(Servo_B, 500, 2500);
  // servo_z.setPeriodHertz(50); //Tần số dùng để đk servo; Thường là 50hz 
  // servo_z.attach(Servo_Z, 500, 2500);  //Sử dụng độ rộng xung min-max với SG90 là 500us-2400us
  //                                 // Với MG996/995 là 1000us - 2000us
  //                                 // Tùy động cơ khác nhau sẽ có độ rộng xung khác nhau
  //                                 //để chạy chính xác từ 0-180 độ
  // servo_kep.setPeriodHertz(50);
  // servo_kep.attach(Servo_kep, 500, 2500); 

  while (!radio.begin()) 
  { Serial.println("Module không khởi động được...!!");  } 

  radio.openWritingPipe(diachi[1]);
  radio.openReadingPipe(1, diachi[0]);
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);  
  
  if (!radio.available())
  {
    Serial.println("Chưa kết nối được với TX...!!");
    Serial.println("CHỜ KẾT NỐI.......");
  }

}

void loop() 
{
  
  if(nhan[9] == 0) //Control: 0-ON, 1-OFF
  {
    dk_kep();
    truc_z();
    truc_a();
    truc_b();
  }

  if(nhan[8] == 1) //auto home
  {
    servo_kep.write(0);
    servo_b.write(115);
    servo_a.write(50);
    servo_z.write(90);
    gui[8] = 0;
  }

  radio.stopListening(); //Ngưng nhận
  tong_lenh();
  doc_servo();
  radio.write(&gui, sizeof(gui)); //Gui
  delay(10);

  radio.startListening(); //Bắt đầu nhận
  while(!radio.available());
  radio.read(&nhan, sizeof(nhan)); //nhan  
  delay(10);

  luu_lenh_moi();

  xoa_lenh();

  chay_lenh();

// Serial.print(" nhan 9: "); Serial.print(nhan[9]);

// Serial.print("  Tong lenh: "); Serial.print(nhan[10]);
// Serial.print("  Delay VT: "); Serial.print(nhan[15]);
// Serial.print("  Delay Lenh: "); Serial.print(nhan[16]);

// Serial.print(" - A: "); Serial.print(nhan[0]); 
// Serial.print("  B: "); Serial.print(nhan[1]);
// Serial.print("  C: "); Serial.print(nhan[2]); 
// Serial.print("  D: "); Serial.print(nhan[3]);
// Serial.print("  E: "); Serial.print(nhan[4]);
// Serial.print("  ---  JOY: "); Serial.print(nhan[5]);
// Serial.print(" --- X: "); Serial.print(nhan[6]); //phai - 100, trai - 0
// Serial.print(" --- Y: "); Serial.print(nhan[7]); //len - 100, xuong - 0    

// Serial.println();
      
}

  /*
  * Nhan:
  * 0-A; 1-B; 2-C; 3-D; 4-E; 5-JOY; 6-X; 7-Y
  * 8-Auto home; 9-Control
  * 10-Tong so lenh; 11- Bien kiem tra luu
  * 12- nhan vi tri;
  * 
  * 14- lenh DIEU KHIEN
  * 17- Lenh DK; 18- So lan thuc hien
  * 
  * 13- Lenh xoa; 20- lenh CAN XOA
  * 
  * 19- Lenh STOP
  * 
  * 15-Delay Vi Tri; 16-Delay Cuoi
  * 
  * Gui: 
  * 
  * 0-A; 1-B; 2-C; 3-KEP; 8-Reset auto home
  * 4-doc eeprom 0/TONG LENH
  * 
  * 13- sau khi XOA xong
  */