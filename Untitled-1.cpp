#include <Servo.h>

#define int1 2 
#define int2 3
#define int3 4
#define int4 5
#define speed 6
#define echo A5
#define trig A6
#define servo 9
#define shoot1 A7
#define shoot2 13
#define ir_L A0
#define ir_R A1
#define ir_M A2
#define ir_LL A3
#define ir_RR A4
#define s0 7
#define s1 8
#define s2 10
#define s3 11
#define out 12

int t=0,dur=0;
bool dir, x=0,n=0;
long time;
int dis;
int redF=0;
int greenF=0;
int blueF=0;
int NOF=0;

Servo reload_servo ;
void color();
void forward();
void backward();
void stop();
void right1();
void left1();
void left();
void right();
void backleft();
void backright();
int ultrasonic();
void red();
void green();
void yellow();

void setup()
{
  TCCR0B = TCCR0B & B11111000 | B00000101;
  pinMode(int1,OUTPUT);
  pinMode(int2, OUTPUT);
  pinMode(int3, OUTPUT); 
  pinMode(int4, OUTPUT);
  pinMode(speed, OUTPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  pinMode ( s0  , OUTPUT ) ;
  pinMode ( s1  , OUTPUT ) ;
  pinMode ( s2 , OUTPUT ) ;
  pinMode ( s3  , OUTPUT ) ;
  pinMode ( out  , INPUT ) ;
  pinMode(shoot1,OUTPUT);
  pinMode(shoot2,OUTPUT);
  digitalWrite ( s0 , HIGH );
  digitalWrite ( s1 , LOW );
  Serial.begin(9600);
  reload_servo.attach(servo); 
  t=millis();
}

void loop()
{
  color();

  dis=ultrasonic(); 
  if (dis > 15 && dis < 40 && x==0){
   stop();      
   Serial.write('O');
   x++;
  }else if (dis < 5 && x==1 ){
   Serial.write('C');
    x=0;
  }
  
   if(analogRead(ir_LL)>450 && analogRead(ir_L)<450 &&analogRead(ir_R)<450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)<450){
    left1();   
  }else if(analogRead(ir_RR)>450 && analogRead(ir_L)<450 &&analogRead(ir_R)<450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)<450){
    right1();  
  }else if(analogRead(ir_L)>450 &&analogRead(ir_R)<450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)>450){    
    left(); 
  }else if(analogRead(ir_L)<450 &&analogRead(ir_R)>450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)>450){
    right(); 
  }else if( analogRead(ir_LL)>450){
    left1(); 
    dir=0;
    delay(30);  
  }else if(analogRead(ir_RR)>450){
    right1();
    dir=1;    
    delay(30);        
  }else if(analogRead(ir_L)>450 &&analogRead(ir_R)<450 && analogRead(ir_M)<450){    
    left1();
  }else if(analogRead(ir_L)<450 &&analogRead(ir_R)>450 && analogRead(ir_M)<450){
    right1();   
  }else if(analogRead(ir_L)<450 &&analogRead(ir_R)<450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)>450){
  forward();
  }  
}
void color(){
  dur=millis()-t;
  if(dur>=200){
  digitalWrite ( s2, LOW );
  digitalWrite ( s3, LOW );
  redF = pulseIn(out, LOW);
  digitalWrite ( s2, HIGH );
  digitalWrite ( s3, HIGH );
  greenF = pulseIn(out, LOW);
  digitalWrite ( s2, HIGH );
  digitalWrite ( s3, LOW );
  NOF = pulseIn(out, LOW);
  digitalWrite ( s2, LOW );
  digitalWrite ( s3, HIGH );
  blueF = pulseIn(out, LOW);
  if (blueF<greenF && redF > blueF && NOF<60 )reload_servo.write (90);
  else if (redF<greenF && redF < blueF && NOF>30)red();
  else if (redF>greenF && greenF < blueF )green();
  else if (redF<greenF && redF < blueF && NOF<30)yellow();
  t=millis();
  }
}
void forward(){
  digitalWrite(int1, HIGH);
  digitalWrite(int2, LOW);
  digitalWrite(int3, HIGH);
  digitalWrite(int4, LOW);
  analogWrite(speed,50);
}
void backward(){
  digitalWrite(int1, LOW);
  digitalWrite(int2, HIGH);
  digitalWrite(int3, LOW);
  digitalWrite(int4, HIGH);
  analogWrite(speed,55);
}
void stop(){
  digitalWrite(int1, LOW);
  digitalWrite(int2, LOW);
  digitalWrite(int3, LOW);
  digitalWrite(int4, LOW);
}
void right1(){
  digitalWrite(int1, HIGH);
  digitalWrite(int2, LOW);
  digitalWrite(int3, LOW);
  digitalWrite(int4, HIGH);
  analogWrite(speed,65);
  
}
void left1(){
  digitalWrite(int1, LOW);
  digitalWrite(int2, HIGH);
  digitalWrite(int3, HIGH);
  digitalWrite(int4, LOW);
  analogWrite(speed,65);
}
void left(){
  digitalWrite(int1, LOW);
  digitalWrite(int2, LOW);
  digitalWrite(int3, HIGH);
  digitalWrite(int4, LOW);
  analogWrite(speed,70);
}
void right(){
  digitalWrite(int1, HIGH);
  digitalWrite(int2, LOW);
  digitalWrite(int3, LOW);
  digitalWrite(int4, LOW);
  analogWrite(speed,70);
}
void backright(){
  digitalWrite(int1, LOW);
  digitalWrite(int2, LOW);
  digitalWrite(int3, LOW);
  digitalWrite(int4, HIGH);
  analogWrite(speed,70);  
}
void backleft(){
  digitalWrite(int1, LOW);
  digitalWrite(int2, HIGH);
  digitalWrite(int3, LOW);
  digitalWrite(int4, LOW);
  analogWrite(speed,70);
}  
int ultrasonic(){
  digitalWrite(trig,0);
  delay(1);
  digitalWrite (trig,1);
  delay(1);
  digitalWrite (trig,0);
  time = pulseIn(echo,1);
  dis = (time/2)/29.1;
  return dis;  
}
void red(){
  stop();  
  reload_servo.write (0);
  digitalWrite(shoot1,HIGH); 
  digitalWrite(shoot2,LOW);  
  delay(1000);
  digitalWrite(shoot1,LOW); 
  digitalWrite(shoot2,LOW);  
  backward();
  delay(10);
  while(analogRead(ir_RR)<450||analogRead(ir_RR)<450){
  if(analogRead(ir_LL)>450 && analogRead(ir_L)<450 &&analogRead(ir_R)<450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)<450){
    left1();   
  }else if(analogRead(ir_RR)>450 && analogRead(ir_L)<450 &&analogRead(ir_R)<450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)<450){
    right1();  
  }else if(analogRead(ir_L)>450 &&analogRead(ir_R)<450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)>450){    
    backleft(); 
  }else if(analogRead(ir_L)<450 &&analogRead(ir_R)>450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)>450){
    backright();
  }else if(analogRead(ir_L)>450 &&analogRead(ir_R)<450 && analogRead(ir_M)<450){    
    left1();
  }else if(analogRead(ir_L)<450 &&analogRead(ir_R)>450 && analogRead(ir_M)<450){
    right1();  
  }else if(analogRead(ir_L)<450 &&analogRead(ir_R)<450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)>450){
  backward();
  } 
  if(dir==1){
  left1();
  delay(30);  
 }else if(dir==0){
  right1();
  delay(30); 
 }
  }
}
void green(){
  stop();
  reload_servo.write (180);
  digitalWrite(shoot1,HIGH); 
  digitalWrite(shoot2,LOW);  
  delay(1000);
  digitalWrite(shoot1,LOW); 
  digitalWrite(shoot2,LOW);  
  backward();
  delay(10);
  while(analogRead(ir_RR)<450||analogRead(ir_RR)<450){
  if(analogRead(ir_LL)>450 && analogRead(ir_L)<450 &&analogRead(ir_R)<450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)<450){
    left1();   
  }else if(analogRead(ir_RR)>450 && analogRead(ir_L)<450 &&analogRead(ir_R)<450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)<450){
    right1();  
  }else if(analogRead(ir_L)>450 &&analogRead(ir_R)<450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)>450){    
    backleft(); 
  }else if(analogRead(ir_L)<450 &&analogRead(ir_R)>450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)>450){
    backright();
  }else if(analogRead(ir_L)>450 &&analogRead(ir_R)<450 && analogRead(ir_M)<450){    
    left1();
  }else if(analogRead(ir_L)<450 &&analogRead(ir_R)>450 && analogRead(ir_M)<450){
    right1();  
  }else if(analogRead(ir_L)<450 &&analogRead(ir_R)<450 && analogRead(ir_LL)<450 && analogRead(ir_RR)<450 && analogRead(ir_M)>450){
  backward();
  } 
  if(dir==1){
  left1();
  delay(30);  
 }else if(dir==0){
  right1();
  delay(30); 
  }
  }
}
void yellow(){
  if(x==0){
  analogWrite(speed,200);
  x=1;
  }  
  else if(x==1) {
    analogWrite(speed,50);
    x=0;    
   }
}