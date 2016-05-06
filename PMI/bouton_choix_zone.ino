int bouton = 2;
int led = 13;

void setup()
{
  Serial.begin(9600);
  pinMode(bouton, INPUT_PULLUP);
  pinMode(led,OUTPUT);
}

void loop()
{

  if(digitalRead(bouton)==HIGH)
  {
    digitalWrite(led,LOW);
    Serial.println(0);
  }
  else
  {
    digitalWrite(led,HIGH);
    Serial.println(1);
  }
}
