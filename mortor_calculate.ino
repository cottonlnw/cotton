#include <ps5Controller.h>
#include <math.h>

// ล้อแมคคานัม
#define IN1 16  // ล้อหน้าซ้าย
#define IN2 17  
#define IN3 18  // ล้อหลังซ้าย
#define IN4 19  
#define ENA_FL 5   // PWM ควบคุมความเร็วล้อหน้าซ้าย
#define ENA_BL 2   // PWM ควบคุมความเร็วล้อหลังซ้าย

#define IN5 21  // ล้อหน้าขวา
#define IN6 22  
#define IN7 23  // ล้อหลังขวา
#define IN8 25  
#define ENB_FR 26  // PWM ควบคุมความเร็วล้อหน้าขวา
#define ENB_BR 27  // PWM ควบคุมความเร็วล้อหลังขวา  

void setup() {
    Serial.begin(115200);
    ps5.begin("14:3A:9A:2F:BE:32");
    Serial.println("Bluetooth Ready.");
    
    // ตั้งค่าขาเป็น OUTPUT
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA_FL, OUTPUT);
    pinMode(ENA_BL, OUTPUT);

    pinMode(IN5, OUTPUT);
    pinMode(IN6, OUTPUT);
    pinMode(IN7, OUTPUT);
    pinMode(IN8, OUTPUT);
    pinMode(ENB_FR, OUTPUT);
    pinMode(ENB_BR, OUTPUT);
}

void loop() {
    int leftX = ps5.LStickX();  // ค่าการเคลื่อนที่แนวข้าง
    int leftY = ps5.LStickY();  // ค่าการเคลื่อนที่เดินหน้า-ถอยหลัง
    int rightX = ps5.RStickX(); // ค่าการหมุนตัว

    // แปลงค่าจาก -128 ถึง 127 เป็น -255 ถึง 255
    int X = map(leftX, -128, 127, -255, 255);
    int Y = map(leftY, -128, 127, -255, 255);
    int Z = map(rightX, -128, 127, -255, 255);

    // คำนวณค่ามอเตอร์ล้อแมคคานัม
    int M1 = Y + X + Z;
    int M2 = Y - X - Z;
    int M3 = Y - X + Z;
    int M4 = Y + X - Z;
  
    if (X > -10 && X < 10) X = 0;
    if (Y > -10 && Y < 10) Y = 0;
    if (Z > -10 && Z < 10) Z = 0;

    // จำกัดค่าความเร็วให้อยู่ในช่วง 0-255
    M1 = constrain(M1, -255, 255);
    M2 = constrain(M2, -255, 255);
    M3 = constrain(M3, -255, 255);
    M4 = constrain(M4, -255, 255);


    // ควบคุมมอเตอร์ล้อหน้าซ้าย
    analogWrite(ENA_FL, abs(M1));
    digitalWrite(IN1, M1 > 0);
    digitalWrite(IN2, M1 < 0);

    // ควบคุมมอเตอร์ล้อหลังซ้าย
    analogWrite(ENA_BL, abs(M2));
    digitalWrite(IN3, M2 > 0);
    digitalWrite(IN4, M2 < 0);

    // ควบคุมมอเตอร์ล้อหน้าขวา
    analogWrite(ENB_FR, abs(M3));
    digitalWrite(IN5, M3 > 0);
    digitalWrite(IN6, M3 < 0);

    // ควบคุมมอเตอร์ล้อหลังขวา
    analogWrite(ENB_BR, abs(M4));
    digitalWrite(IN7, M4 > 0);
    digitalWrite(IN8, M4 < 0);
}