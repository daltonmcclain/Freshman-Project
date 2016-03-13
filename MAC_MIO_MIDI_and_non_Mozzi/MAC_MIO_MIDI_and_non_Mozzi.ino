#include <Servo.h>

int IRVal;
int servoVal = 180;
Servo servo1;
int var;
int noteON = 144;
int noteOFF = 128;

void setup() {
  Serial.begin(31250);
  pinMode(2,INPUT);
  pinMode(4,INPUT);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(13,INPUT);
}

void loop() {
  
  servo1.attach(3);
  IRVal = analogRead(A2);
  servo1.write(servoVal);
  if (IRVal < 350 && IRVal > 100){
    if (servoVal > 30){
      servoVal = servoVal - 1;
      delay(50);
    }
  }
  if (IRVal < 600 && IRVal > 350){
    if (servoVal < 180){
      servoVal = servoVal + 1;
      delay(50);
    }
  }
  
  if (digitalRead(2) == LOW){
    analogWrite(9,0);
    analogWrite(10,0);
    analogWrite(11,0);
    analogWrite(random(9,12),255);
    var = analogRead(A1);
    var = map(var,0,1023,-39,39);
    MIDImessage(noteON, 60 + var, 127);
    delay(300);
    MIDImessage(noteOFF, 0, 0);
  }
  
  if (digitalRead(4) == LOW){
    analogWrite(9,0);
    analogWrite(10,0);
    analogWrite(11,0);
    analogWrite(random(9,12),255);
    var = analogRead(A1);
    var = map(var,0,1023,-39,39);
    MIDImessage(noteON, 62 + var, 127);
    delay(300);
    MIDImessage(noteOFF, 0, 0);
  }
  
  if (digitalRead(7) == LOW){
    analogWrite(9,0);
    analogWrite(10,0);
    analogWrite(11,0);
    analogWrite(random(9,12),255);
    var = analogRead(A1);
    var = map(var,0,1023,-39,39);
    MIDImessage(noteON, 64 + var, 127);
    delay(300);
    MIDImessage(noteOFF, 0, 0);
  }
  
  if (digitalRead(8) == LOW){
    analogWrite(9,0);
    analogWrite(10,0);
    analogWrite(11,0);
    analogWrite(random(9,12),255);
    var = analogRead(A1);
    var = map(var,0,1023,-39,39);
    MIDImessage(noteON, 65 + var, 127);
    delay(300);
    MIDImessage(noteOFF, 0, 0);
  }
  
  if (digitalRead(12) == LOW){
    analogWrite(9,0);
    analogWrite(10,0);
    analogWrite(11,0);
    analogWrite(random(9,12),255);
    var = analogRead(A1);
    var = map(var,0,1023,-39,39);
    MIDImessage(noteON, 67 + var, 127);
    delay(300);
    MIDImessage(noteOFF, 0, 0);
  }
  
  if (digitalRead(13) == LOW){
    analogWrite(9,0);
    analogWrite(10,0);
    analogWrite(11,0);
    analogWrite(random(9,12),255);
    var = analogRead(A1);
    var = map(var,0,1023,-39,39);
    MIDImessage(noteON, 60 + var, 127);
    delay(300);
    MIDImessage(noteOFF, 0, 0);
    delay(200);
    MIDImessage(noteON, 62 + var, 127);
    delay(300);
    MIDImessage(noteOFF, 0, 0);
    delay(200);
    MIDImessage(noteON, 64 + var, 127);
    delay(300);
    MIDImessage(noteOFF, 0, 0);
    delay(200);
    MIDImessage(noteON, 65 + var, 127);
    delay(300);
    MIDImessage(noteOFF, 0, 0);
    delay(200);
    MIDImessage(noteON, 67 + var, 127);
    delay(300);
    MIDImessage(noteOFF, 0, 0);
  }
}

void MIDImessage(int command, int MIDInote, int MIDIservoVal) {
  Serial.write(command); 
  Serial.write(MIDInote);
  Serial.write(MIDIservoVal);
}
