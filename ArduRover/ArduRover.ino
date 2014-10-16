#include <Wire.h>
#include "nunchuck_funcs.h"

#define MOTOR_A 12
#define SPEED_A 3
#define BRAKE_A 9

#define MOTOR_B 13
#define SPEED_B 11
#define BRAKE_B 8

#define SERIAL_ON 0
#define WIICHUCK_ON 1

//    tophat left x 24
//    tophat left y 124
//    
//    tophat right x 216
//    tophat right y 124
//    
//    tophat up x 124
//    tophat up y 218
//    
//    tophat down x 120
//    tophat down y 30
//
//    tophat center x 122
//    tophat center y 125


int loop_cnt=0;

byte accx, accy, zbut, cbut, joyx, joyy;
int ledPin = 13;

//255 is maximum speed
int leftspeed = 255;
int rightspeed = 255;


// from http://arduino.cc/en/Main/ArduinoMotorShieldR3
//Function	    Channel A	Channel B
//Direction	    Digital 12	Digital 13
//Speed (PWM)	    Digital 3	Digital 11
//Brake	            Digital 9	Digital 8
//Current Sensing   Analog 0	Analog 1

void setup() {
    //Setup Channel A
    pinMode(12, OUTPUT); //Initiates Motor Channel A pin
    pinMode(9, OUTPUT);  //Initiates Brake Channel A pin
    pinMode(3, OUTPUT);  //Initiates Speed Channel A pin
  
    //setup Channel B
    pinMode(13, OUTPUT); //Initiates Motor Channel B pin
    pinMode(8, OUTPUT);  //Initiates Speed Channel B pin
    pinMode(11, OUTPUT); //Initiates Brake Channel B pin
  
    Serial.begin(19200);
    Serial.println("setup");
    
    if (WIICHUCK_ON) {
        nunchuck_setpowerpins();
        nunchuck_init(); // send the initilization handshake
    }
}

void loop() {
    if (SERIAL_ON && !WIICHUCK_ON) {
      serialOn();
    }
    
    if (!SERIAL_ON && WIICHUCK_ON) {
      wiiChuck();
    }
}

void wiiChuck() {
    if( loop_cnt > 100) { // every 100 msecs get new data
        loop_cnt = 0;

        nunchuck_get_data();
        //nunchuck_print_data();
        
        joyx = nunchuck_joyx() - 127;
        joyy = nunchuck_joyy() - 127;
        
        Serial.println(joyy);
        
        joyx = (joyx);
        joyy = (joyy);        
        
        if (isLeft()) {
          Serial.println("to the left");
          left(leftspeed,rightspeed);
        } else if (isRight()) {
          Serial.println("to the right");
          right(leftspeed,rightspeed);
        } else if (isForward()) {
          Serial.println("forward ho");
          forward(leftspeed,rightspeed);
        } else if (isReverse()) {
          Serial.println("retreat");
          reverse(leftspeed,rightspeed);
        } else if (isCenter()) {
          Serial.println("in the center");
          stop();
        } else {
          stop();
        }
        
//        debugChuck();
    }

    loop_cnt++;
    delay(1);
}

boolean isLeft() {
  return joyx >= 150 && joyx < 250;// joyx >= 20 && joyx <= 122 && joyy >= 120 && joyy <=126;
}

boolean isRight() {
  return joyx >= 10 && joyx <150;//joyx >= 122 && joyx <= 220 && joyy >= 122 && joyy <= 135;
}

boolean isCenter() {
  return joyx <= 255 && joyx >= 250;
}

boolean isForward() {
  return joyy >= 0 && joyy <= 95;//joyx >=120 && joyx <=124 && joyy >=122 && joyy <= 225;
}

boolean isReverse() {
  return joyy >= 160 && joyy < 250;//false;//joyx >= 120 && joyx <= 124 && joyy >= 25 && joyy <=126;
}

void debugChuck() {
    accx  = nunchuck_accelx(); // ranges from approx 70 - 182
    accy  = nunchuck_accely(); // ranges from approx 65 - 173
    zbut = nunchuck_zbutton();
    cbut = nunchuck_cbutton(); 
    
    Serial.print("accx: "); Serial.print((byte)accx,DEC);
    Serial.print("\taccy: "); Serial.print((byte)accy,DEC);
    Serial.print("\tzbut: "); Serial.print((byte)zbut,DEC);
    Serial.print("\tcbut: "); Serial.println((byte)cbut,DEC);
}

void serialOn() {
    // Wait until a character is received
    while (Serial.available() < 1) {} 
    
    char val = Serial.read();


    // Perform an action depending on the command
    switch(val) {
        case 'w'://Move Forward
        case 'W':
            forward (leftspeed,rightspeed);
            break;
        case 's'://Move Backwards
        case 'S':
            reverse (leftspeed,rightspeed);
            break;
        case 'a'://Turn Left
        case 'A':
            left (leftspeed,rightspeed);
            break;
        case 'd'://Turn Right
        case 'D':
            right (leftspeed,rightspeed);
            break;
        default:
            stop();
            break;
    }
}

void stop(void) {
  digitalWrite(MOTOR_A, LOW);
  analogWrite(SPEED_A, 0);
  digitalWrite(MOTOR_B, LOW);
  analogWrite(SPEED_B, 0);
  Serial.println("stop");
}

void forward(char a, char b) {
  analogWrite (SPEED_A,a);
  digitalWrite(MOTOR_A,LOW);
  analogWrite (SPEED_B,b);
  digitalWrite(MOTOR_B,LOW);
  Serial.println("forward");
}

void reverse (char a, char b) {
  analogWrite (SPEED_A,a);
  digitalWrite(MOTOR_A,HIGH);
  analogWrite (SPEED_B,b);
  digitalWrite(MOTOR_B,HIGH);
  Serial.println("reverse");
}

void left (char a, char b) {
  analogWrite (SPEED_A, a);
  digitalWrite(MOTOR_A, LOW);
  analogWrite (SPEED_B, b);
  digitalWrite(MOTOR_B, HIGH);
  Serial.println("left");
}

void right (char a, char b) {
  analogWrite (SPEED_A, a);
  digitalWrite(MOTOR_A, HIGH);
  analogWrite (SPEED_B, b);
  digitalWrite(MOTOR_B, LOW);
  Serial.println("right");
}

