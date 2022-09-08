#include <SPI.h>
#include <LoRa.h>

#define moistureSensor A0

#define CS 10
#define RESET 9
#define DIO0 2

int moist;
int limit = 500;
String header = "X";

void setup()
{
  Serial.begin (9600);

  pinMode (moistureSensor, INPUT);

  LoRa.setPins (CS, RESET, DIO0);
  if (!LoRa.begin(433E6))
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop()
{
  moist = analogRead (moistureSensor);
  Serial.print ("Kelembapan Tanah : ");
  Serial.println (moist);

  while (LoRa.beginPacket() == 0)
  {
    Serial.print("waiting for radio ... ");
    delay(100);
  }

  Serial.println("Sending packet: ");

  LoRa.beginPacket();
  LoRa.print (header);
  LoRa.print ("#");
  LoRa.print (moist);
  LoRa.print ("#");
  LoRa.endPacket();
}
