#include <SPI.h>
#include <LoRa.h>
#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 0 //DHT pin D0
#define DHTTYPE DHT11
LiquidCrystal lcd(8, 3, 4, 5, 6, 7);
DHT dht(DHTPIN, DHTTYPE);

void LoRaReceiveAndTampilkanLCD();
int LoRaSend(String dataSensor);
String receivedData = "";
String receivedDataTemp = "";
String temp = "";
String dataSensor;
void setup() {
  lcd.begin(16, 2);
  dht.begin();
  Serial.begin(9600);
  while (!Serial);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  dataSensor = "";
  for (int i = 0;i<10000;i++) {
    LoRaReceiveAndTampilkanLCD();
  }
  dataSensor += "T:";
  dataSensor += dht.readTemperature();
  dataSensor += " | H:";
  dataSensor += dht.readHumidity();
  LoRaSend(dataSensor);
  //delay(2000);
  lcd.setCursor(0,0);
  lcd.print(dataSensor); 
  lcd.setCursor(0,1);
  lcd.print(receivedData); 
delay(1000);
}




void LoRaReceiveAndTampilkanLCD() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("\nReceived packet '");

    while (LoRa.available()) {
      temp = (char)LoRa.read();
      Serial.print(temp);
      receivedDataTemp += temp;
      //Serial.print((char)LoRa.read());
    }
    receivedData = receivedDataTemp ;
    receivedDataTemp = "";

    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}

int LoRaSend(String data) {
  Serial.print("Sending packet: ");
  LoRa.beginPacket();
  LoRa.print(data);
  LoRa.endPacket();
  Serial.println(data);
}

