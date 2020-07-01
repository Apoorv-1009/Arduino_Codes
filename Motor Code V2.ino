#define JoyX A0
#define JoyY A1
#define Button 2
#define FL 7 //Top Left Wheel
#define FR 8 //Top Right Wheel
#define BL 3  //Bottom Left Wheel (PWM Pin)
#define BR 5  //Bottom Right Wheel (PWM Pin)
void setup() 
{
  // put your setup code here, to run once:
  pinMode(JoyX,INPUT);
  pinMode(JoyY,INPUT);
  Serial.begin(9600);
}
void Drive(int DirL, int DirR, int Oct0, int Oct2, int a, int b, int p, int q, int X, int Y)
{
  digitalWrite(FL,DirL);    //Left Direction
  digitalWrite(FR,DirR);    //Right Direction
  analogWrite(BL, abs(255*Oct0 -abs(X*a) - abs(Y*b)));    //Left Pwm
  analogWrite(BR, abs(255*Oct2 -abs(X*p) - abs(Y*q)));    //Right Pwm
}
int buffer(int k)
{
    if(abs(k) < 20)
      k = 0;
    if(k > 230)
      k = 255;
    if(k < -230)
      k = -255;
    return k;
}
void loop() 
{
  int x,y;
  // put your main code here, to run repeatedly:
  x = analogRead(JoyX);
  y = analogRead(JoyY);
  x = map(x,0,1023,-255,255);
  y = map(y,0,1023,-255,255);
  x = buffer(x);
  y = buffer(y);
  Serial.print(x);
  Serial.print("  ");
  Serial.println(y);
  if(x < 10 && y<10)    //No Motion
    Drive(0,0,0,0,0,0,0,0,x,y);
    
  else if(abs(x) < 10 && y < 0)   //Forward
    Drive(1,1,0,0,0,1,0,1,x,y);
    
  else if(abs(x) < 10 && y > 0)   //Backward
    Drive(0,0,0,0,0,1,0,1,x,y);
    
  else if(abs(y)<20 && x > 0)   //Spot Turn Right
    Drive(1,0,0,0,1,0,1,0,x,y);
    
  else if(abs(y)<20 && x < 0)   //Spot Turn Left
    Drive(0,1,0,0,1,0,1,0,x,y);
    
  else if(x>0 && y <0 && x>=abs(y))   //Octet 1
    Drive(1,0,0,1,1,0,0,1,x,y);
    
  else if(x>0 && y<0 && abs(y)>x)  //Octet 2
    Drive(1,1,0,1,0,1,1,0,x,y);
    
  else if(x<0 && y<0 && abs(y)>abs(x))  //Octet 3
    Drive(1,1,1,0,1,0,0,1,x,y);
    
  else if(x<0 && y<0 && abs(x)>=abs(y)) //Octet 4
    Drive(0,1,1,0,0,1,1,0,x,y);
  
  else if(x<0 && y>0 && abs(x)>y) //octet 5
    Drive(0,1,0,1,1,0,0,1,x,y);

  else if(x<0 && y>0 && y>=abs(x)) //octet 6
    Drive(0,0,0,1,0,1,-1,0,x,y);
  
  else if(x>0 && y>0 && y>=abs(x))  //octet 7
    Drive(0,0,1,0,1,0,0,1,x,y);
    
  else if(x>0 && y>0 && x>y)  //octet 8
    Drive(1,0,1,0,0,1,1,0,x,y);
}










      
