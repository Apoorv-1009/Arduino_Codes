#define UF 13;
#define UR 12;
#define UL 11;
#define UFi 2;
#define URi 3;
#define ULi 4;
#define L 6;
#define R 7;
void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
   pinMode(2,INPUT);
   pinMode(3,INPUT);
    pinMode(4,INPUT);
}
int d1,t1;
int d2,t2;
int d3,t3;
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(UF,HIGH);
  digitalWrite(UR,HIGH);
  digitalWrite(UL,HIGH);
  delayMicroseconds(10);
  digitalWrite(UF,LOW);
  digitalWrite(UR,LOW);
  digitalWrite(UL,LOW);
  t1=pulseIn(UFi,HIGH);
  t2=pulseIn(URi,HIGH);
  t3=pulseIn(ULi,HIGH);
  d1=(0.0343*t1)/2; 
  d2=(0.0343*t2)/2;
  d3=(0.0343*t3)/2;
  if(d1<40)
  {
    //MOTION STOP
    if(d2>d3)
      digitalWrite( L,HIGH); //TURN LEFT
      
    if(d2<d3)
      digitalWrite(R,HIGH); //TURN RIGHT

    if((d2<10)&&(d3<10))
      digitalWrite(L,HIGH);
      digitalWrite(R,HIGH);
  }
   digitalWrite( L,LOW);
   digitalWrite( R,LOW);
}
