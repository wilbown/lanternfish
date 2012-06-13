//#include <TimerOne.h>

/* 3DLP printer control
 
*/

int stepCount = 0;         // number of steps the motor has taken

void setup() {
  Serial.begin(57600);
  
  pinMode(11,OUTPUT); //enable
  pinMode(7,OUTPUT); //direction
  pinMode(6,OUTPUT); //step
  
  pinMode(13,OUTPUT);
  
  //pinMode(2,INPUT);
  //digitalWrite(2,HIGH); //use internal 20K pullup resister
  
//***master
//  Serial.println("S"); //resets back to the first slide
//  delay(1000);
}

//int state13 = LOW;
int in = -1;
int c = 8;
int var = 0;


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
      digitalWrite(13, LOW); //LED
  
      digitalWrite(13, LOW); //LED
      
      Serial.println("N"); //show next slide
      stepCount++;
    } else if (in==49) { //trigger 1
      digitalWrite(13, LOW);
    } else if (in==50) { //trigger 2
      digitalWrite(13, HIGH);
    } else if (in==51) { //trigger 3
      //TEST
      digitalWrite(13, HIGH); //LED
      
      
      digitalWrite(11, HIGH); //Enable
      delay(1000);
      
      
      digitalWrite(7, HIGH); //Direction UP
      delay(1000);
      
      var = 0;
      while (var < 2000){
        digitalWrite(6, HIGH); //Step
        delay(1);
        digitalWrite(6, LOW); //reset Step
        delay(1);
        var++;
      }
      
      digitalWrite(7, LOW); //Direction DOWN
      delay(1000);
      
      var = 0;
      while (var < 2000){
        digitalWrite(6, HIGH); //Step
        delay(1);
        digitalWrite(6, LOW); //reset Step
        delay(1);
        var++;
      }
      
      
      digitalWrite(11, LOW); //Disable
  
      digitalWrite(13, LOW); //LED
      
    }
  }

//***just testing switch
//  if (digitalRead(2)==HIGH) digitalWrite(13, LOW);
//  else digitalWrite(13, HIGH);
}
