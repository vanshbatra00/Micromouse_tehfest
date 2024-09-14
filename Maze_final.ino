#define Kp 2
#define Ki 2
#define Kd 2

//Libraries
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
Adafruit_MPU6050 mpu;

float yaw = 0;
float leftmotorspeed = 80;
float rightmotorspeed = 80;

//PID variables
float lasterror = 0;
float errorsum=0;
float error =0;
unsigned long timer = 0;

//Encoder variables
int IR_encoder=23; 
int count = 0;
float rev = 0;
float x = 0;
int past_dist = 0;

// motor A pins
int motor1 = 32;
int motor2 = 33;
 int en1 = 25;

// motor B pins
int motor3 = 14;
int motor4 = 27;
 int en2 = 12;

// ir sensor pins
int left_ir = 34;
int mid_ir_1 = 35;
int right_ir = 39;

//CLASSES
class first_array{
public:
  int arr[20][20]={};
};

first_array obj;

//array to store coordinate
//int arr[20][20]={0};

//pointers
int p = 0;
int q = 0;
//starting in north
char direction = 'N';

//End position variables
int turn_left_count=0;
int turn_right_count=0;
int value=0;   //Left=2 & Right=3
int store_p,store_q;

// Forward
void move_forward(){
 analogWrite(en1, leftmotorspeed);
 analogWrite(en2, rightmotorspeed);
 digitalWrite(motor1, HIGH);
 digitalWrite(motor2, LOW);
 digitalWrite(motor3, HIGH);
 digitalWrite(motor4, LOW);
  
  //storing the correct path
  //int x;
  if ( (x-past_dist) >= 16){
    if (direction == 'N'){
      q = q+1;
      if (obj.arr[p][q] == 0){
        obj.arr[p][q] = 1;
      }
      else if(obj.arr[p][q] == 1){
        obj.arr[p][q] = 2;
      }
    }
    else if (direction == 'S'){
      q = q-1;
      if (obj.arr[p][q] == 0){
        obj.arr[p][q] = 1;
      }
      else if (obj.arr[p][q] == 1){
        obj.arr[p][q] = 2;
      }
    }
    else if (direction == 'E'){
      p = p+1;
      if (obj.arr[p][q] == 0){
        obj.arr[p][q] = 1;
      }
      else if (obj.arr[p][q] == 1){
        obj.arr[p][q] = 2;
      }
    }                
    else if (direction == 'W'){
      p = p-1;
      if (obj.arr[p][q] == 0){
        obj.arr[p][q] = 1;
      }
      else if (obj.arr[p][q] == 1){
            obj.arr[p][q] = 2;
      }
    }
  }
}

//Turn left
void turn_left(){
  analogWrite(en1,80);
  analogWrite(en2,80);
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, HIGH);
  digitalWrite(motor3, HIGH);
  digitalWrite(motor4, LOW);

  //changing direction
  if (direction == 'N'){
    direction = 'W';
  }
  else if (direction == 'S'){
    direction = 'E';
  }
  else if (direction == 'E'){
    direction = 'N';
  }
  else if (direction == 'W'){
    direction = 'S';
  }
}

//Turn right
void turn_right(){
  analogWrite(en1, 80);
  analogWrite(en2, 80);
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, HIGH);

  //changing direction
  if (direction == 'N'){
    direction = 'E';
  }
  else if (direction == 'S'){
    direction = 'W';
  }
  else if (direction == 'E'){
    direction = 'S';
  }
  else if (direction == 'W'){
    direction = 'N';
  }
}

void u_turn(){
  analogWrite(en1, 80);
  analogWrite(en2, 80);
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, HIGH);
  digitalWrite(motor3, HIGH);
  digitalWrite(motor4, LOW);

  //changing direction
  if (direction == 'N'){
    direction = 'S';
  }
  else if (direction == 'S'){
    direction = 'N';
  }
  else if (direction == 'E'){
    direction = 'W';
  }
  else if (direction == 'W'){
    direction = 'E';
  }
}

//Stop
void stop(){
  analogWrite(en1,0);
  analogWrite(en2,0);
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, LOW);
}

//PID_Left and Right
void PID_L()
{
     float errorsum = errorsum + error;

     float motorspeed = error*Kp + errorsum*Ki + (error - errorsum)*Kd;
     lasterror = error;

     rightmotorspeed = 255 - motorspeed;
     leftmotorspeed = 255 + motorspeed;

     rightmotorspeed = constrain(rightmotorspeed, 0, 255);  
     leftmotorspeed = constrain(leftmotorspeed, 0, 255);
}

void PID_R()
{
     float errorsum = errorsum + error;

     float motorspeed = error*Kp + errorsum*Ki + (error - errorsum)*Kd;
     lasterror = error;

     rightmotorspeed = 255 + motorspeed;
     leftmotorspeed = 255 - motorspeed;

     rightmotorspeed = constrain(rightmotorspeed, 0, 255);  
     leftmotorspeed = constrain(leftmotorspeed, 0, 255);
}

//distance
// void interruptcount(){
//   interrupt++;

// }


void setup() {
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(en1, OUTPUT);

  pinMode(motor3, OUTPUT);
  pinMode(motor4, OUTPUT);
  pinMode(en2, OUTPUT);
  
  pinMode(left_ir, INPUT);
  pinMode(mid_ir_1, INPUT);
  pinMode(right_ir, INPUT);
  
  // Turn off motors - Initial state
  analogWrite(en1,0);
  analogWrite(en2,0);
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, LOW);

  Serial.begin(9600);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  
  pinMode(IR_encoder,INPUT);

  //starting point
  obj.arr[0][0] = 1;
}

void loop() {
  //digitalRead(left_ir) Getting left IR value(0 or 1)
  //digitalRead(mid_ir_1) Getting middle IR value(0 or 1)
  //digitalRead(right_ir) Getting right IR value(0 or 1)

  //ESP32 touch
  if(touchRead(A0)<12)    //With human touch value goes at <10 
  {
    obj.arr[0][0]=1;
    p=0;
    q=0;
    yaw=0;
    leftmotorspeed=255;
    rightmotorspeed=255;
    errorsum=0;
    error=0;
    timer=0;
    count=0;
    rev=0;
    x=0;
    past_dist=0;
    turn_left_count=0;
    turn_right_count=0;
    value=0;
  }

  //END POINT
  if( (turn_left_count==1 && value==2) || (turn_right_count==1 && value==3) )
  {
    store_p=p;
    store_q=q;
  }

  if(turn_left_count==3 || turn_right_count==3){
    if( (store_p==p) && (store_q==q+1 || store_q==q-1) ){
      stop();
    }

    if( (store_p==p-1 || store_p==p+1) && (store_q==q) ){
      stop();
    }
  }
  
  //MPU YAW
  sensors_event_t a,g,temp;
  mpu.getEvent(&a, &g, &temp);
  yaw = yaw + g.gyro.z;

  if (g.gyro.z > 0){
   error = yaw;
   PID_L();
  }
  else if(g.gyro.z < 0){
  error = yaw;
  PID_R();
  }
  
  //Controlling robot direction based on IR value
  if (direction == 'N'){
    //only left path available
    if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == LOW) && ((obj.arr[p-1][q] == 0) || (obj.arr[p-1][q] == 1))){
      turn_left();
      if(value==2){
        turn_left_count = turn_left_count+1;
      }
      else{
        value=2;
        turn_left_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == LOW) && (obj.arr[p-1][q] == 2)){
      u_turn();
    }
    //left && straight
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW) && (obj.arr[p-1][q] == 0)){
      turn_left();
      if(value==2){
        turn_left_count = turn_left_count+1;
      }
      else{
        value=2;
        turn_left_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW) && (obj.arr[p-1][q] == 1)){
      if (obj.arr[p][q+1] == 0){
        move_forward();
      }
      else if ((obj.arr[p][q+1] == 1) || (obj.arr[p][q+1] == 2)){
        turn_left();
        if(value==2){
          turn_left_count = turn_left_count+1;
        }
        else{
          value=2;
          turn_left_count = 1;
        }
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW) && (obj.arr[p-1][q] == 2)){
      if ((obj.arr[p][q+1] == 0) || (obj.arr[p][q+1] == 1)){
        move_forward();
      }
      else if (obj.arr[p][q+1] == 2){
        u_turn();
      }
    }
    //left && right available
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p-1][q] == 0)){
      turn_left();
      if(value==2){
        turn_left_count = turn_left_count+1;
      }
      else{
        value=2;
        turn_left_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p-1][q] == 1)){
      if (obj.arr[p+1][q] == 0){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if ((obj.arr[p+1][q] == 1) || (obj.arr[p+1][q] == 2)){
        turn_left();
        if(value==2){
          turn_left_count = turn_left_count+1;
        }
        else{
          value=2;
          turn_left_count = 1;
        }
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p-1][q] == 2)){
      if ((obj.arr[p+1][q] == 0) || (obj.arr[p+1][q] == 1)){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if (obj.arr[p+1][q] == 2){
        u_turn();
      }
    }     
    //only straight   
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW)){
      move_forward();
    }
    //straight && right
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == HIGH) && (obj.arr[p][q+1] == 0)){
      move_forward();
    }
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == HIGH) && (obj.arr[p][q+1] == 1)){
      if (obj.arr[p+1][q] == 0){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if ((obj.arr[p+1][q] == 1) || (obj.arr[p+1][q] == 2)){
        move_forward();
      }
    }
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == HIGH) && (obj.arr[p][q+1] == 2)){
      if ((obj.arr[p+1][q] == 0) || (obj.arr[p+1][q] == 1)){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if (obj.arr[p+1][q] == 2){
        u_turn();
      }
    }
    //only right 
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && ((obj.arr[p+1][q] == 0) || (obj.arr[p+1][q] == 1))){
      turn_right();
      if(value==3){
        turn_right_count = turn_right_count+1;
      }
      else{
        value=3;
        turn_right_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1)  == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p+1][q] == 2)){
      u_turn();
    }
  }
    
  else if (direction == 'S'){
    //only left path available
    if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == LOW) && ((obj.arr[p+1][q] == 0) || (obj.arr[p+1][q] == 1))){
      turn_left();
      if(value==2){
        turn_left_count = turn_left_count+1;
      }
      else{
        value=2;
        turn_left_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == LOW) && (obj.arr[p+1][q] == 2)){
      u_turn();
    }
    //left && straight
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW) && (obj.arr[p+1][q] == 0)){
      turn_left();
      if(value==2){
        turn_left_count = turn_left_count+1;
      }
      else{
        value=2;
        turn_left_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW) && (obj.arr[p+1][q] == 1)){
      if (obj.arr[p][q-1] == 0){
        move_forward();
      }
      else if ((obj.arr[p][q-1] == 1) || (obj.arr[p][q-1] == 2)){
        turn_left();
        if(value==2){
          turn_left_count = turn_left_count+1;
        }
        else{
          value=2;
          turn_left_count = 1;
        }
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW) && (obj.arr[p+1][q] == 2)){
      if ((obj.arr[p][q-1] == 0) || (obj.arr[p][q-1] == 1)){
        move_forward();
      }
      else if (obj.arr[p][q-1] == 2){
        u_turn();
      }
    }
    //left && right available
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p+1][q] == 0)){
      turn_left();
      if(value==2){
        turn_left_count = turn_left_count+1;
      }
      else{
        value=2;
        turn_left_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p+1][q] == 1)){
      if (obj.arr[p-1][q] == 0){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if ((obj.arr[p-1][q] == 1) || (obj.arr[p-1][q] == 2)){
        turn_left();
        if(value==2){
          turn_left_count = turn_left_count+1;
        }
        else{
          value=2;
          turn_left_count = 1;
        }
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p+1][q] == 2)){
      if ((obj.arr[p-1][q] == 0) || (obj.arr[p-1][q] == 1)){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if (obj.arr[p-1][q] == 2){
        u_turn();
      }
    }
    //only straight   
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW)){
      move_forward();
    }
    //straight && right
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == HIGH) && (obj.arr[p][q-1] == 0)){
      move_forward();
    }
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == HIGH) && (obj.arr[p][q-1] == 1)){
      if (obj.arr[p-1][q] == 0){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if ((obj.arr[p-1][q] == 1) || (obj.arr[p-1][q] == 2)){
        move_forward();
      }
    }
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == HIGH) && (obj.arr[p][q-1] == 2)){
      if ((obj.arr[p-1][q] == 0) || (obj.arr[p-1][q] == 1)){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if (obj.arr[p-1][q] == 2){
        u_turn();
      }
    }
    //only right 
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && ((obj.arr[p-1][q] == 0) || (obj.arr[p-1][q] == 1))){
      turn_right();
      if(value==3){
        turn_right_count = turn_right_count+1;
      }
      else{
        value=3;
        turn_right_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p-1][q] == 2)){
      u_turn();
    }
  }

  else if (direction == 'E'){
    //only left path available
    if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == LOW) && ((obj.arr[p][q+1] == 0) || (obj.arr[p][q+1] == 1))){
      turn_left();
      if(value==2){
        turn_left_count = turn_left_count+1;
      }
      else{
        value=2;
        turn_left_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == LOW) && (obj.arr[p][q+1] == 2)){
      u_turn();
    }
    //left && straight
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW) && (obj.arr[p][q+1] == 0)){
      turn_left();
      if(value==2){
        turn_left_count = turn_left_count+1;
      }
      else{
        value=2;
        turn_left_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW) && (obj.arr[p][q+1] == 1)){
      if (obj.arr[p+1][q] == 0){
        move_forward();
      }
      else if ((obj.arr[p+1][q] == 1) || (obj.arr[p+1][q] == 2)){
        turn_left();
        if(value==2){
          turn_left_count = turn_left_count+1;
        }
        else{
          value=2;
          turn_left_count = 1;
        }
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW) && (obj.arr[p][q+1] == 2)){
      if ((obj.arr[p+1][q] == 0) || (obj.arr[p+1][q] == 1)){
        move_forward();
      }
      else if (obj.arr[p+1][q] == 2){
        u_turn();
      }
    }
    //left && right available
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p][q+1] == 0)){
      turn_left();
      if(value==2){
        turn_left_count = turn_left_count+1;
      }
      else{
        value=2;
        turn_left_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p][q+1] == 1)){
      if (obj.arr[p][q-1] == 0){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if ((obj.arr[p][q-1] == 1) || (obj.arr[p][q-1] == 2)){
        turn_left();
              if(value==2){
          turn_left_count = turn_left_count+1;
        }
        else{
          value=2;
          turn_left_count = 1;
        }
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p][q+1] == 2)){
      if ((obj.arr[p][q-1] == 0) || (obj.arr[p][q-1] == 1)){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if (obj.arr[p][q-1] == 2){
        u_turn();
      }
    }
    //only straight   
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW)){
      move_forward();
    }
    //straight && right
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == HIGH) && (obj.arr[p+1][q] == 0)){
      move_forward();
    }
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == HIGH) && (obj.arr[p+1][q] == 1)){
      if (obj.arr[p][q-1] == 0){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if ((obj.arr[p][q-1] == 1) || (obj.arr[p][q-1] == 2)){
        move_forward();
      }
    }
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == HIGH) && (obj.arr[p+1][q] == 2)){
      if ((obj.arr[p][q-1] == 0) || (obj.arr[p][q-1] == 1)){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if (obj.arr[p][q-1] == 2){
        u_turn();
      }
    }
    //only right 
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && ((obj.arr[p][q-1] == 0) || (obj.arr[p][q-1] == 1))){
      turn_right();
      if(value==3){
        turn_right_count = turn_right_count+1;
      }
      else{
        value=3;
        turn_right_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p][q-1] == 2)){
      u_turn();
    }
  }

  else if (direction == 'W'){
    //only left path available
    if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == LOW) && ((obj.arr[p][q-1] == 0) || (obj.arr[p][q-1] == 1))){
      turn_left();
      if(value==2){
        turn_left_count = turn_left_count+1;
      }
      else{
        value=2;
        turn_left_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == LOW) && (obj.arr[p][q-1] == 2)){
      u_turn();
    }
    //left && straight
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW) && (obj.arr[p][q-1] == 0)){
      turn_left();
      if(value==2){
        turn_left_count = turn_left_count+1;
      }
      else{
        value=2;
        turn_left_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW) && (obj.arr[p][q-1] == 1)){
      if (obj.arr[p-1][q] == 0){
        move_forward();
      }
      else if ((obj.arr[p-1][q] == 1) || (obj.arr[p-1][q] == 2)){
        turn_left();
        if(value==2){
          turn_left_count = turn_left_count+1;
        }
        else{
          value=2;
          turn_left_count = 1;
       }
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW) && (obj.arr[p][q-1] == 2)){
      if ((obj.arr[p-1][q] == 0) || (obj.arr[p-1][q] == 1)){
        move_forward();
      }
      else if (obj.arr[p-1][q] == 2){
        u_turn();
      }
    }
    //left && right available
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p][q-1] == 0)){
      turn_left();
      if(value==2){
        turn_left_count = turn_left_count+1;
      }
      else{
        value=2;
        turn_left_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p][q-1] == 1)){
      if (obj.arr[p][q+1] == 0){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if ((obj.arr[p][q+1] == 1) || (obj.arr[p][q+1] == 2)){
        turn_left();
        if(value==2){
          turn_left_count = turn_left_count+1;
        }
        else{
          value=2;
          turn_left_count = 1;
        }
      }
    }
    else if ((digitalRead(left_ir) == HIGH) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p][q-1] == 2)){
      if ((obj.arr[p][q+1] == 0) || (obj.arr[p][q+1] == 1)){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if (obj.arr[p][q+1] == 2){
        u_turn();
      }
    }     
    //only straight   
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == LOW)){
      move_forward();
    }
    //straight && right
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == HIGH) && (obj.arr[p-1][q] == 0)){
      move_forward();
    }
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == HIGH) && (obj.arr[p-1][q] == 1)){
      if (obj.arr[p][q+1] == 0){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if ((obj.arr[p][q+1] == 1) || (obj.arr[p][q+1] == 2)){
        move_forward();
      }
    }
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == HIGH) && (digitalRead(right_ir) == HIGH) && (obj.arr[p-1][q] == 2)){
      if ((obj.arr[p][q+1] == 0) || (obj.arr[p][q+1] == 1)){
        turn_right();
        if(value==3){
          turn_right_count = turn_right_count+1;
        }
        else{
          value=3;
          turn_right_count = 1;
        }
      }
      else if (obj.arr[p][q+1] == 2){
        u_turn();
      }
    }
    //only right 
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && ((obj.arr[p][q+1] == 0) || (obj.arr[p][q+1] == 1))){
      turn_right();
      if(value==3){
        turn_right_count = turn_right_count+1;
      }
      else{
        value=3;
        turn_right_count = 1;
      }
    }
    else if ((digitalRead(left_ir) == LOW) && (digitalRead(mid_ir_1) == LOW) && (digitalRead(right_ir) == HIGH) && (obj.arr[p][q+1] == 2)){
      u_turn();
    }
  }

  //ENCODER
  if(count==5)
  {
    rev = rev+1;
    Serial.print("Revolution: ");
    Serial.print(rev);
    Serial.print("   Distance: ");
    x = rev*3.1415*6;
    Serial.println(x);
    past_dist = x;
    count=0;
  }

  int value = digitalRead(IR_encoder);
  
  if(value==0){
    count = count+1;    
  }

  value=0;
}
