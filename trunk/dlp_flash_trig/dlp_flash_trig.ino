//#include <TimerOne.h>

/* 3DLP printer control
 
*/

float stepH = 0.008466;         // number of steps per mm

void setup() {
  Serial.begin(57600);
  
  pinMode(11,OUTPUT); //enable
  pinMode(7,OUTPUT); //direction
  pinMode(6,OUTPUT); //step
  
  pinMode(13,OUTPUT); //LED
  
  pinMode(2,INPUT); //home limit switch
  digitalWrite(2,HIGH); //use internal 20K pullup resister
  
  pinMode(4,INPUT); //kill switch
  digitalWrite(4,HIGH); //use internal 20K pullup resister
  
//***master
//  Serial.println("S"); //resets back to the first slide
//  delay(1000);
}

//int state13 = LOW;
//int c = 8;
int in = -1;
boolean kill = false;


void loop() {
//***master
//  if (c<0) return;
//  c--;
//  if (c==-1) {
//    Serial.println("D"); //done, close
//    return;
//  }
//  Serial.println("N"); //show next slide
//  delay(800);
//  Serial.println("B"); //show a black slide
//  delay(200);
  
//***slave
  if (Serial.available() > 0) {
    in = Serial.read();
    if (in==48) { //next position
    
      stepENABLE();
      long steps;
      
      stepUP();
      steps = 2.0/stepH;
      long i = 0;
      while (i < steps) {
        if (stepKILL()) break; //kill
        if (digitalRead(2) == HIGH) break;
        stepone();
        i++;
      }
      
      stepDOWN();
      steps = 1.5/stepH;
      long j = 0;
      while (j < steps) {
        if (stepKILL()) break; //kill
        if (digitalRead(2) == HIGH) break;
        stepone();
        j++;
      }
      
      stepDISABLE();
  
      
      delay(1000);
      
      Serial.println("N"); //show next slide
    } else if (in==49) { //trigger 1
    
      stepENABLE();
      
      stepUP();
      
      long steps = 10/stepH;
      
      long i = 0;
      while (i < steps) {
        if (stepKILL()) break; //kill
        if (digitalRead(2) == HIGH) break;
        stepone();
        i++;
      }
      
      stepDISABLE();
      
    } else if (in==50) { //trigger 2
    
      stepENABLE();
      
//      long i = 0;
//      while (i < 500) {
//        if (stepKILL()) break; //kill
//        if (digitalRead(2) == HIGH) break;
//        stepUP();
//        stepone();
//        stepDOWN();
//        stepone();
//        i++;
//      }
      
      stepDISABLE();
      
    } else if (in==51) { //trigger 3
      //TEST
      stepENABLE();
      
      stepHOME();
      
      stepDISABLE();
      
    }
  }

//***just testing switch
//  if (digitalRead(2)==HIGH) digitalWrite(13, LOW);
//  else digitalWrite(13, HIGH);
}

void stepHOME() {
    stepDOWN();
    long i = 0;
    while (i < 45000) {
      if (stepKILL()) break; //kill
      if (digitalRead(2) == HIGH) {
        stepUP();
        long j = 0;
        while (j < 800) {
          if (stepKILL()) break; //kill
          if (digitalRead(2) == LOW) break;
          stepone();
          j++;
        }
        break;
      }
      stepone();
      i++;
    }
}

boolean stepKILL() {
  if (digitalRead(4) == HIGH) return true;
  return false;
}
void stepENABLE() {
  digitalWrite(13, HIGH); //LED
  digitalWrite(11, HIGH); //Enable
  delay(100);
}
void stepDISABLE() {
  digitalWrite(11, LOW); //Disable
  digitalWrite(13, LOW); //LED
}

void stepUP() {
  digitalWrite(7, HIGH); //Direction UP
  delay(100);
}
void stepDOWN() {
  digitalWrite(7, LOW); //Direction DOWN
  delay(100);
}

void stepone() {
  digitalWrite(6, HIGH); //Step
  delayMicroseconds(1000);
  //delay(1);
  digitalWrite(6, LOW); //reset Step
  //delay(10);
}

//set up interrupt pins 2,3 for kill switches
//attachInterrupt()
