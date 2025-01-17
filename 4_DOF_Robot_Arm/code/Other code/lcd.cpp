/*

CÁC HÀM THÔNG DỤNG KHI DÙNG I2C

lcd.init()  : Khởi tạo màn hình lcd
lcd.clear() : Xóa màn hình, đưa con trỏ về vị trí 0,0
lcd.home()  : Đưa con trỏ về vị trí 0
lcd.setCursor(x,y)  : Đưa con trỏ về vị trí cột thứ x, hàng thứ y
lcd.noDisplay() : Không hiển thị gì trên màn hình cả
lcd.display() : Hiển thị lên màn hình
lcd.noBlink() : Không nhấp nháy con trỏ
lcd.blink() : Nhấp nháy con trỏ
lcd.scrollDisplayLeft() : Cuộn màn hình bên trái
lcd.scrollDisplayRight()  : Cuộn màn hình bên phải
lcd.leftToRight() : Kí tự sẽ hiển thị từ phía bên trái sang phải
lcd.rightToLeft() : Kí tự sẽ hiển thị từ phía phải sang trái
lcd.noBacklight() : Tắt đèn nền
lcd.backlight() : Có đèn nền
lcd.write(x)  : Lcd sẽ in ra kí tự thứ x trong ô nhớ từ 0-7 của CGRAM
lcd.createChar(location,charmap[])  : Hiển thị một kí tự charmap[] bất kì
 

*/

#include <Wire.h> //Gọi thư viện I2C để sử dụng các thư viện I2C

#include <LiquidCrystal_I2C.h> //Thư viện LCD I2C
LiquidCrystal_I2C lcd(0x27,16,2); //Khai báo địa chỉ I2C (0x27 or 0x3F) và LCD 16x02

void setup() 
{
  lcd.init(); //Khởi tạo màn hình LCD
  lcd.backlight(); //Bật đèn màn hình lCD

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
