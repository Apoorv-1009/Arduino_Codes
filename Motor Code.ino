#define JoyX A0
#define JoyY A1
#define Button 2
#define FL 11 //Top Left Wheel
#define FR 6 //Top Right Wheel
#define BL 3  //Bottom Left Wheel 
#define BR 5  //Bottom Right Wheel
/*void drive(Xval,Yval)
{
  analogWrite(W1,Xval) 
  analogWrite(W3,Xval)
  analogWrite(W2,Yval)
  analogWrite(W4,Yval)
}*/
void setup() 
{
  // put your setup code here, to run once:
  pinMode(JoyX,INPUT);
  pinMode(JoyY,INPUT);
  pinMode(Button,INPUT);
  Serial.begin(9600);
}
void loop() 
{
  // put your main code here, to run repeatedly:
  int X,Y,x,y;
  int x1,y1,Xval,Yval;
  X = analogRead(JoyX);
  Y = analogRead(JoyY);
  //Change of Origin
  x = X - 512;
  y = Y - 512;
  if(abs(x) <= 5)
    x = 0;
  if(abs(y) <= 5)
    y = 0;
  if(abs(x) >= 509 and x > 0)
  {
    x = 512;
  }
  if(abs(y) >= 509 and y > 0)
  {
    y = 512;
  }
  Serial.print(x);
  Serial.print("  ");
  Serial.println(y);
  
  if(x==0 && y==0)
  {
    analogWrite(FL,0); 
    analogWrite(BL,0);
    analogWrite(FR,0);
    analogWrite(BR,0);
    Serial.println("No Motion");
    
  }
  //Octet 1 
  if( x > 0 && y < 0 && abs(x) > abs(y))
  {
    y1 = abs(y);
    Xval = map(x,0,511,0,255);
    Yval = map(y1,0,511,255,0);
    analogWrite(FL,255); 
    analogWrite(BL,Xval);
    analogWrite(FR,0);
    analogWrite(BR,Yval);
    Serial.println("Octet 1");
  }
  
  //Octet 2
  if( x > 0 && y < 0 && abs(x) < abs(y))
  {
    x1 = abs(x);
    y1 = abs(y);
    Xval = map(x,0,511,255,0);
    Yval = map(y1,0,511,0,255);
    analogWrite(FL,255); 
    analogWrite(BL,Yval);
    analogWrite(FR,255);
    analogWrite(BR,Xval);
    Serial.println("Octet 2");
  }
  //Octet 3
  if( x < 0 && y < 0 && abs(x) < abs(y))
  {
    x1 = abs(x);
    y1 = abs(y);
    Xval = map(x1,0,511,255,0);
    Yval = map(y1,0,511,0,255);
    analogWrite(FL,255); 
    analogWrite(BL,Xval);
    analogWrite(FR,255);
    analogWrite(BR,Yval);
    Serial.println("Octet 3");
  }
  //Octet 4
  if( x < 0 && y < 0 && abs(x) > abs(y))
  {
    x1 = abs(x);
    y1 = abs(y);
    Xval = map(x1,0,511,0,255);
    Yval = map(y1,0,511,255,0);
    analogWrite(FL,0); 
    analogWrite(BL,Yval);
    analogWrite(FR,255);
    analogWrite(BR,Xval);
    Serial.println("Octet 4");
  }  
  // Octet 5
  if( x < 0 && y > 0 && abs(x) > abs(y))
  {
    x1 = abs(x);
    y1 = abs(y);
    Xval = map(x1,0,511,0,255);
    Yval = map(y1,0,511,255,0);
    analogWrite(FL,0); 
    analogWrite(BL,Xval);
    analogWrite(FR,255);
    analogWrite(BR,Yval);
    Serial.println("Octet 5");
  }  
  //Octet 6
  if( x < 0 && y > 0 && abs(x) < abs(y))
  {
    x1 = abs(x);
    y1 = abs(y);
    Xval = map(x1,0,511,255,0);
    Yval = map(y1,0,511,0,255);
    analogWrite(FL,0); 
    analogWrite(BL,Yval);
    analogWrite(FR,0);
    analogWrite(BR,Xval);
    Serial.println("Octet 6");
  }  
  //Octet 7
  if( x > 0 && y > 0 && x < y)
  {
    x1 = abs(x);
    y1 = abs(y);
    Xval = map(x1,0,511,255,0);
    Yval = map(y1,0,511,0,255);
    analogWrite(FL,0); 
    analogWrite(BL,Xval);
    analogWrite(FR,0);
    analogWrite(BR,Yval);
    Serial.println("Octet 7");
  }  
  //Octet 8
  if( x > 0 && y > 0 && abs(x) > abs(y))
  {
    x1 = abs(x);
    y1 = abs(y);
    Xval = map(x1,0,511,0,255);
    Yval = map(y1,0,511,255,0);
    analogWrite(FL,255); 
    analogWrite(BL,Yval);
    analogWrite(FR,0);
    analogWrite(BR,Xval);
    Serial.println("Octet 8");
  }  
  //General Motion
  if( x == 0 && y > 0) 
  {
    y1 = abs(y);
    Yval = map(y1,0,511,0,255);
    analogWrite(FL,0); 
    analogWrite(BL,Yval);
    analogWrite(FR,0);
    analogWrite(BR,Yval);
    Serial.println("Backward");
  }
  if( x == 0 && y < 0) 
  {
    y1 = abs(y);
    Yval = map(y1,0,511,0,255);
    analogWrite(FL,255); 
    analogWrite(BL,Yval);
    analogWrite(FR,255);
    analogWrite(BR,Yval);
    Serial.println("Forward");
  }
  if(y == 0 && x < 0)
  {
    x1 = abs(x);
    Xval = map(x1,0,511,0,255);
    analogWrite(FL,0); 
    analogWrite(BL,Xval);
    analogWrite(FR,255);
    analogWrite(BR,Xval);
    Serial.println("Spot Turn Left");
  }
  if(y == 0 && x > 0)
  {
    Xval = map(x,0,511,0,255);
    analogWrite(FL,255); 
    analogWrite(BL,Xval);
    analogWrite(FR,0);
    analogWrite(BR,Xval);
    Serial.println("Spot Turn Right");
  }
  if(x == y)
  {
    if(x < 0)
    {
      y1 = abs(y);
      Yval = map(y1,0,511,0,255);
      analogWrite(FL,0); 
      analogWrite(BL,0);
      analogWrite(FR,255);
      analogWrite(BR,Yval);
      Serial.println("Left Front");
    }
    if(x > 0)
    {
      x1 = abs(x);
      Xval = map(x1,0,511,0,255);
      analogWrite(FL,0); 
      analogWrite(BL,0);
      analogWrite(FR,0);
      analogWrite(BR,Xval);
      Serial.println("Right Back");
    }
  }
    if(x == -y && x < 0) 
    {
      Yval = map(y,0,511,0,255);
      analogWrite(FL,0); 
      analogWrite(BL,Yval);
      analogWrite(FR,0);
      analogWrite(BR,0);
      Serial.println("Left Back");
    }
    if(x == -y && x > 0) 
    {
      Xval = map(x,0,511,0,255);
      analogWrite(FL,255); 
      analogWrite(BL,Xval);
      analogWrite(FR,0);
      analogWrite(BR,0);
      Serial.println("Right Front");
    }
}




  
 
