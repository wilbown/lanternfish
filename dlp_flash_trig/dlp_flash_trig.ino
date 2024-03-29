//#include <TimerOne.h>

/* 3DLP printer control
 
*/

float stepH = 0.008466;         // number of steps per mm

void setup() {
  Serial.begin(57600);
  pinMode(8,OUTPUT); //ultrasonic control (HIGH = off)
  digitalWrite(8,HIGH);
  
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
boolean sercom = false;

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
    sercom = true;
    in = Serial.read();
  }
  
  if (stepKILL()) {
    sercom = false; //throw away serial
    return;
  }
  
  if (sercom) {
    sercom = false;
    if (in==48) { //next position
      stepENABLE();
      long steps;
      
      stepUP();
      steps = 10.0/stepH;
      long i = 0;
      while (i < steps) {
        if (stepKILL()) break; //kill
        if (digitalRead(2) == HIGH) break;
        steponeSlow();
        i++;
      }
      
      delay(15000);
      
      stepDOWN();
      steps = 9.9/stepH;
      long j = 0;
      while (j < steps) {
        if (stepKILL()) break; //kill
        if (digitalRead(2) == HIGH) break;
        stepone();
        j++;
      }
      
      stepDISABLE();
      digitalWrite(8,HIGH); //let ultrasonic cool down
      delay(30000);
      digitalWrite(8,LOW);
      
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
      
      stepUP();
      long steps = 170/stepH;
      long i = 0;
      while (i < steps) {
        if (stepKILL()) break; //kill
        if (digitalRead(2) == HIGH) break;
        stepone();
        i++;
      }
//      long steps;
//      long i;
//      
//      stepUP();
//      steps = 100/stepH;
//      i = 0;
//      while (i < steps) {
//        if (stepKILL()) break; //kill
//        if (digitalRead(2) == HIGH) break;
//        stepone();
//        i++;
//      }
//      stepDOWN();
//      steps = 100/stepH;
//      i = 0;
//      while (i < steps) {
//        if (stepKILL()) break; //kill
//        if (digitalRead(2) == HIGH) break;
//        stepone();
//        i++;
//      }
      
      stepDISABLE();
      
    } else if (in==51) { //trigger 3
      //HOME
      stepENABLE();
      
      stepHOME();
      
      stepDISABLE();
      
    }
  }

//***just testing switch
//  if (digitalRead(2)==HIGH) digitalWrite(13, LOW);
//  else digitalWrite(13, HIGH);
}

boolean kill = false;
boolean stepKILL() {
  if (digitalRead(4) == HIGH) {
    if (!kill) {
      digitalWrite(8,HIGH); //turn off ultrasonic
      Serial.println("S"); //stop AIR app
    }
    kill = true;
    return true;
  }
  if (kill) digitalWrite(8,LOW); //turn on ultrasonic
  kill = false;
  return false;
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
  delayMicroseconds(3000);
  //delay(1);
  digitalWrite(6, LOW); //reset Step
  //delay(10);
}
void steponeSlow() {
  digitalWrite(6, HIGH); //Step
  delay(50);
  //delay(1);
  digitalWrite(6, LOW); //reset Step
  //delay(10);
}

//set up interrupt pins 2,3 for kill switches
//attachInterrupt()
//kill = true;
