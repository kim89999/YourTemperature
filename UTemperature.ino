#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Wire.h> // I2C LCD 

#include <Servo.h>
Servo servoLeft;
Servo servoRight; // 서보

#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); //비접촉식 적외선 온도측정센서

int led = 5; //경고 LED

void setup()
{
  servoLeft.attach(13);
  servoRight.attach(12);
  pinMode(10, INPUT); pinMode(9, OUTPUT); 
  pinMode(3, INPUT); pinMode(2, OUTPUT); //서보모터 입출력
  pinMode(5, OUTPUT); //LED 출력
  mlx.begin();  
}

void loop()
{
 int irLeft = irDetect(9, 10, 38000);
 int irRight = irDetect(2, 3, 38000);

 if((irLeft == 0) && (irRight == 0)) // 적외선센서에 감지될 경우 정지
 {  
   servoLeft.writeMicroseconds(1500); 
   servoRight.writeMicroseconds(1500);  
   LCD1();
   
   if (mlx.readAmbientTempC() >= 22){ //일정 온도가 넘으면 경고LED와 부저가 울림
    tone(4, 1000, 1000);
    digitalWrite(5,HIGH);
    lcd.print("temp:");
    lcd.print(mlx.readAmbientTempC());
    lcd.setCursor(0, 1);
    lcd.print("can't get in.TT ");
   }
 } 
 else
 {  
   servoLeft.writeMicroseconds(1400); 
   servoRight.writeMicroseconds(1400); // 적외선 센서에 감지되지 않았을 경우
   LCD2();  
   digitalWrite(5,LOW);
 }
}
int LCD1() //
{   
    lcd.begin();
    lcd.setCursor(0, 0);
    lcd.print("temp:");
    lcd.print(mlx.readAmbientTempC());
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Have a nice day.");
    lcd.noBlink();
}
int LCD2() //
{
  lcd.begin();
  lcd.print("I'm on way.");
  lcd.setCursor(0, 1);
  lcd.print("so please wait.");  
  lcd.noBlink();
}

int irDetect(int irPin, int irReceiverPin, long frequency) //적외선센서를 위한 함수를 가져옴
{
  tone(irPin, frequency, 8);
  delay(1); 
  int ir = digitalRead(irReceiverPin);
  delay(1);
  return ir;
}
