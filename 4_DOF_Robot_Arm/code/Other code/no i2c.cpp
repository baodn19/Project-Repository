
/*
 *Chân VSS, RW, K - GND
 *Chân VDD, A - 5v
 *V0 - chân tín hiệu biến trở
 *RS - 12 
 *E - 11
 *D4 - 5
 *D5 - 4
 *D6 - 3
 *D7 - 2
*/

#include <LiquidCrystal.h> //Thư viện dùng LCD, có sẵn trong Arduino
#include <Arduino.h>
 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //Các chân điều khiển LCD

void setup() 
{
  lcd.begin(20, 4); //Khai báo LCD 16x02

}

void loop() 
{
  lcd.setCursor(0, 0); //Chọn vị trí đặt con trỏ. 0 đầu là chọn cột (từ 0 - 16), 0 kế là chọn hàng (có 2 hàng 0 và 1)
  lcd.print("Green Technology"); //in lên lcd chữ tỏng dấu ""
  delay(5000); // chờ 5giay
  lcd.clear(); //Xóa LCD
  //lcd.noDisplay(); //Tắt LCD
  delay(1000);
  
  //lcd.display(); //Mở LCD
  lcd.setCursor(1, 1); // Chọn cột 1 và hàng 1
  lcd.print("I LOVE ARDUINO");
  delay(5000);
  //lcd.noDisplay();
  lcd.clear();
  delay(1000);

  //lcd.display();
  lcd.setCursor(0, 0);
  lcd.print("Green Technology");
  lcd.setCursor(1, 1);
  lcd.print("I LOVE ARDUINO");
  delay(5000);
  //lcd.noDisplay();
  lcd.clear();
  delay(1000);  
}
