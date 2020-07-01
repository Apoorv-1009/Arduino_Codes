//With pulseIn():
/*
void setup()
{
  pinMode (9,INPUT);
  pinMode (10,OUTPUT);
  Serial.begin(9600);
}
  
float t,d;
void loop()
{
  
  digitalWrite(10,HIGH);
  delayMicroseconds(10);
  digitalWrite(10,LOW);
  t=pulseIn(9, HIGH);
  d=(t*0.0343)/2;
  Serial.println(d);
  delay(300);
}
*/
//Without pulseIn():
/*
void setup()
{
  pinMode (9,INPUT);
  pinMode (10,OUTPUT);
  Serial.begin(9600);
}
double a,b,t,d;
void loop()
{
  
  digitalWrite(10,HIGH);
  delayMicroseconds(10);
  digitalWrite(10,LOW);
     
  while(digitalRead(9)!=HIGH)
  {;}
 
  a=micros();
  while(digitalRead(9)!=LOW)
  {
    ;
  }
  b=micros();
  t=b-a;
  d=(t*0.0343)/2;
  Serial.println(d);
  delay(300);
}
*/
