#include <ps5Controller.h>
#define motorLF1 4
#define motorLF2 2
#define motorLFPMW 15

#define motorRF1 32
#define motorRF2 33
#define motorRFPMW 35

#define motorLB1 27
#define motorLB2 26
#define motorLBPMW 25

#define motorRB1 12
#define motorRB2 14
#define motorRBPMW 13

void setup() {
  Serial.begin(115200);
  ps5.begin("14:3a:9a:31:00:13"); //replace with MAC address of your controller
  Serial.println("Ready.");

  pinMode(motorLF1, OUTPUT);
  pinMode(motorLF2, OUTPUT);
  pinMode(motorLFPMW, OUTPUT);

  pinMode(motorRF1, OUTPUT);
  pinMode(motorRF2, OUTPUT);
  pinMode(motorRFPMW, OUTPUT);

  pinMode(motorLB1, OUTPUT);
  pinMode(motorLB2, OUTPUT);
  pinMode(motorLBPMW, OUTPUT);

  pinMode(motorRB1, OUTPUT);
  pinMode(motorRB2, OUTPUT);
  pinMode(motorRBPMW, OUTPUT);
}

void loop() {
  if (ps5.isConnected()) {
    Serial.println("Connected!");
  }

  // Controller inputs
  int strafe = ps5.RStickX(); 
  int speed = ps5.RStickY(); 
  int turn = ps5.LStickX();
  
  // Calculate motor speeds
  int motor1Speed = speed + turn + strafe;    
  int motor2Speed = speed - turn - strafe;
  int motor3Speed = speed + turn - strafe;
  int motor4Speed = speed - turn + strafe;

  // Normalize speeds
  motor1Speed = adjust_speed(motor1Speed);
  motor2Speed = adjust_speed(motor2Speed);
  motor3Speed = adjust_speed(motor3Speed);
  motor4Speed = adjust_speed(motor4Speed);

  int maxMotorSpeed = max(abs(motor1Speed), max(abs(motor2Speed), abs(motor3Speed)));
  if (maxMotorSpeed > 128) {
    motor1Speed = (motor1Speed * 128) / maxMotorSpeed;
    motor2Speed = (motor2Speed * 128) / maxMotorSpeed;
    motor3Speed = (motor3Speed * 128) / maxMotorSpeed;
  }

  // Control motors
  ControlMotor(motor1Speed, motorLF1, motorLF2, motorLFPMW);     
  ControlMotor(motor2Speed, motorRF1, motorRF2, motorRFPMW); 
  ControlMotor(motor3Speed, motorLB1, motorLB2, motorLBPMW);
  ControlMotor(motor4Speed, motorRB1, motorRB2, motorRBPMW);

  delay(20);
}

void ControlMotor(int speed, int A, int B, int PWM) {
  if (speed > 0) { 
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
  } else if (speed < 0) {
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
  } else { 
    digitalWrite(A, LOW);
    digitalWrite(B, LOW);
  }

  int frequency = map(abs(speed), 0, 128, 0, 400);
  if (frequency > 255) {
    frequency = 255;
  }
  analogWrite(PWM, frequency);
}

int adjust_speed(int speed) {
  if (abs(speed) < 10) { 
    return 0;
  }
  return speed;
}


