#include <SparkFun_TB6612.h>  //Motor driver lib

#define m1_L 4  //m1
#define m1_R 2 
#define m1_S 3
#define m2_L 8  //m2
#define m2_R 7
#define m2_S 6
#define STBY 9

#define ENCA1 2 // YELLOW
#define ENCA2 11 // YELLOW
#define ENCB1 3 // WHITE
#define ENCB2 12 // WHITE

volatile int initialposa= 0;
volatile int initialposb= 0;
const int offsetA = -1;
const int offsetB = -1;

Motor motor1 = Motor(m1_L, m1_R, m1_S, offsetA, STBY);
Motor motor2 = Motor(m2_L, m2_R, m2_S, offsetB, STBY);


void setup() {
  Serial.begin(9600);
  pinMode(ENCA1,INPUT);
  pinMode(ENCA2,INPUT);
  pinMode(ENCB1,INPUT);
  pinMode(ENCB2,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA1),readEncoderm1,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCB1),readEncoderm2,RISING);
}

void readEncoderm1(){
  int b = digitalRead(ENCA2);
  if(b > 0){
    initialposa++;
  }
  else{
    initialposa--;
  }
}

void readEncoderm2(){
  int c = digitalRead(ENCB2);
  if(c > 0){
    initialposb++;
  }
  else{
    initialposb--;
  }
}

void loop() {

  int targeta = 20;
  int targetb = 2006;
  int posa = 0;
  int posb = 0;
   
  noInterrupts(); // disable interrupts temporarily while reading
  posa = initialposa;
  posb = initialposb;
  interrupts();
  int ea= targeta-posa;
  int eb=targetb-posa;

  Serial.println(posa);
  Serial.println(posb);

  if (ea !=0 && eb!=0)
  {
    forward(motor1, motor2, 255);
  }
  // else if(eb!=0)
  // {
  //   motor2.drive(255,1000);
  // }
  // else if(ea!=0)
  //   {
  //   motor1.drive(255,1000);
  // }
  else
  {
    brake(motor1, motor2);
    delay(20000);
  }
}