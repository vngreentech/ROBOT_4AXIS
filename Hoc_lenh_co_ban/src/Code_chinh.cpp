#include "thuvien.h"

void setup() 
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();  
  man_hinh();

  servo_A.attach(servoA);
  servo_B.attach(servoB);
  servo_Z.attach(servoZ);
  servo_kep.attach(servokep);

  pinMode(bientro_A, INPUT);
  pinMode(bientro_B, INPUT);
  pinMode(bientro_Z, INPUT);
  pinMode(nutnhan, INPUT_PULLUP);

  pinMode(len, INPUT_PULLUP);
  pinMode(xuong, INPUT_PULLUP);
  pinMode(enter, INPUT_PULLUP);
}

void loop() 
{
  gt_nutnhan = digitalRead(nutnhan);
  gt_enter = digitalRead(enter); 
  gt_len = digitalRead(len);
  gt_xuong = digitalRead(xuong);
  
  if(gt_enter != macdinh1) //Nut ENTER
  {
    if(gt_enter == 0) dem_menu+=1;

      if(dem_menu == 1) menu(); //Vao menu chinh

      if(dem_menu == 2 && dem_lenxuong == 0) //Chon BACK ve man hinh
      {
        lcd.clear();
        man_hinh();
        dem_menu = 0; dem_lenxuong = 0;
      }

      if(dem_menu == 2 && dem_lenxuong == 2) //Chon xoa lenh
      {
        xoa_lenh();
      }

      if(dem_menu == 2 && dem_lenxuong == 1) //Chon them lenh moi
      {
        Them_lenh();
      }

      if(dem_menu == 3 && dem_lenxuong == 1) //Thoat khoi THEM LENH
      {
        EEPROM.write(1,1); //Luu so 1 vao o 1 de xac nhan da co lenh duoc luu
        diachi_cuoi = diachi_Kep;
        EEPROM.write(2, diachi_cuoi); //Luu dia chi cuoi cung vao o so 2
        // Serial.println(EEPROM.read(2));
        
        lcd.clear();
        man_hinh();
        dem_menu = 0;
        dem_lenxuong = 0;
        vitri = 1;
        diachi_A = 6; diachi_B = 7; diachi_Z = 8; diachi_Kep = 9;
      }

      if(dem_menu == 2 && dem_lenxuong == 3) //Chon CHAY LENH
      {
        run = true;
      }

      if(dem_menu == 3 && dem_lenxuong == 3)
      {
        run = false;
        batdau = 10;
        ketthuc = 0;
        dem_chay = 0; dem_menu = 0; dem_lenxuong = 0; 
        lcd.clear();
        man_hinh();          
      }

    macdinh1 = gt_enter;
  }

  if(gt_len != macdinh2) //NUT LEN
  {
    if(gt_len == 0 && dem_menu == 1) //Di chuyen o MENU
    {
      dem_lenxuong-=1;
      if(dem_lenxuong<0)  dem_lenxuong=3;
      menu();
    }

    if(gt_len == 0 && dem_menu == 2) //Neu o MENU them lenh thi nhan nut len de them VI TRI cua lenh
    {      
      luu_lenh();
      vitri += 1;
      lcd.setCursor(12,1);
      lcd.print(vitri);      
      
      // Serial.print("DC A: "); Serial.print(diachi_A); Serial.print("  - DC B: "); Serial.print(diachi_B);
      // Serial.print("  - DC Z: "); Serial.print(diachi_Z); Serial.print("  - DC KEP: "); Serial.println(diachi_Kep);

      // Xem_lenh();
    }

    macdinh2 = gt_len;
  } 
 
  if(gt_xuong != macdinh3) //NUT XUONG
  {
    if(gt_xuong == 0 && dem_menu == 1) //Di chuyen o MENU
    {
      dem_lenxuong+=1;
      if(dem_lenxuong>3) dem_lenxuong=0;
      menu();
    }
    macdinh3 = gt_xuong;
  }

  if(dem_menu == 0 && dem_lenxuong == 0) //Tai MAN HINH
  {
    dk_servo();
    HT_goc(0);
  } 
  else if(dem_menu == 2 && dem_lenxuong == 1) //Tai MENU THEM LENH
  {
    dk_servo();
    HT_goc(2);
  }  
  
  if(run == true && dem_menu == 2 && dem_lenxuong == 3) //CHAY LENH
  {
    chay_lenh();
  }

  // Serial.print("BT_A: "); Serial.print(gt_bientro_A); Serial.print(" - GOC A: "); Serial.print(goc_A);
  // Serial.print("  | BT_B: "); Serial.print(gt_bientro_B); Serial.print(" - GOC B: "); Serial.print(goc_B);
  // Serial.print("  | BT_Z: "); Serial.print(gt_bientro_Z); Serial.print(" - GOC Z: "); Serial.print(goc_Z);
  // Serial.print("  | biendem: "); Serial.print(biendem);

  Serial.print("Len: "); Serial.print(gt_len); Serial.print("  - Xuong: "); Serial.print(gt_xuong); Serial.print("  - Enter: "); Serial.print(gt_enter);
  Serial.print(" | Dem_MENU: "); Serial.print(dem_menu); Serial.print(" - Dem_LX: "); Serial.print(dem_lenxuong);
  Serial.print(" - RUN: "); Serial.print(run);
  Serial.print(" - BatDau: "); Serial.print(batdau); Serial.print(" - KetThuc: "); Serial.print(ketthuc); 
  Serial.print(" - TIME: "); Serial.print(time);
  Serial.print(" - MILLIS: "); Serial.print(millis());

  Serial.println();
  delay(30);
}