#include <SharpIR.h>


//Sensor pins
int s1 =A0; 
int s2 =A1;
int s3 =A2;
int s4 =A3;
int s5 =A4;
int s6 =A5;

int d1,d2,d3,d4,d5,d6; //
SharpIR sensor( SharpIR::GPY0A21YK0F, A0 );
SharpIR sensor1( SharpIR::GPY0A21YK0F, A1 );
SharpIR sensor2( SharpIR::GPY0A21YK0F, A2 );
SharpIR sensor3( SharpIR::GPY0A21YK0F, A3 );
SharpIR sensor4( SharpIR::GPY0A21YK0F, A4 );
SharpIR sensor5( SharpIR::GPY0A21YK0F, A5 );


//motor pins
int m1_l =4;  ///m1
int m1_r =2; 
int m1_s =3;
int m2_l =8;  ///m2
int m2_r =7;
int m2_s =6;

//Sensor values



int arr[50][50];
int arr1[50][50];
int arr2[50][50];
int arr3[50][50];

void setup() {
// put your setup code here, to run once:
//motors
pinMode(m1_l,OUTPUT);
pinMode(m1_r,OUTPUT);
pinMode(m1_s,OUTPUT);
pinMode(m2_l,OUTPUT);
pinMode(m2_r,OUTPUT);
pinMode(m2_s,OUTPUT);

//Sensors
pinMode(s1,INPUT);
pinMode(s2,INPUT);
pinMode(s3,INPUT);
pinMode(s4,INPUT);
pinMode(s5,INPUT);
pinMode(s6,INPUT);

Serial.begin(9600);

}
void read()
{
 d1 = sensor.getDistance();
 d2 = sensor1.getDistance();
 d3 = sensor2.getDistance();
 d4 = sensor3.getDistance();
 d5 = sensor4.getDistance();
 d6 = sensor5.getDistance();
 

}

void forward()
{
  analogWrite(m1_s, 80); 
  analogWrite(m2_s, 80); 
  digitalWrite(m1_l, HIGH);
  digitalWrite(m1_r, LOW);
  digitalWrite(m2_l, HIGH);
  digitalWrite(m2_r, LOW);
}

void backward()
{
  analogWrite(m1_s, 80); 
  analogWrite(m2_s, 80); 
  digitalWrite(m1_r, HIGH);
  digitalWrite(m1_l, LOW);
  digitalWrite(m2_r, HIGH);
  digitalWrite(m2_l, LOW);

}

void stop()
{
  analogWrite(m1_s, 80); 
  analogWrite(m2_s, 80); 
  digitalWrite(m1_r, HIGH);
  digitalWrite(m1_l, LOW);
  digitalWrite(m2_r, HIGH);
  digitalWrite(m2_l, LOW);

}

void loop() {
// put your main code here, to run repeatedly:
  
if
}
