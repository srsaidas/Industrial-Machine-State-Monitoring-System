#include "dh.h"
#include "Arduino.h"

DHT11::DHT11(char pin)
{
  data_pin = pin;
}

void DHT11::start()
{
  pinMode(data_pin,OUTPUT);
  digitalWrite(data_pin,LOW);
  delay(18);// 18ms low
  digitalWrite(data_pin,HIGH);
  delayMicroseconds(20);//20 us
  pinMode(data_pin,INPUT);
}

void DHT11::get_responce()
{
  while(digitalRead(data_pin));
  while(!digitalRead(data_pin));
  while(digitalRead(data_pin)); 
}

char DHT11::read()
{
  char i, data = 0;
  for (i = 0; i < 8 ; i++)
  {
   while(!digitalRead(data_pin));
   delayMicroseconds(30);
   if (digitalRead(data_pin))
   {
    data = data << 1 |1;
   }
   else
   {
    data = data << 1;
   }
   while(digitalRead(data_pin));
  }
  return data;
}
