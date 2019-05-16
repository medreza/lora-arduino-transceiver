#include <SPI.h>
#include <LoRa.h>
#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 0 //DHT pin D0
#define DHTTYPE DHT11
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
DHT dht(DHTPIN, DHTTYPE);

void LoRaReceiveAndTampilkanLCD();
int LoRaSend(String dataSensor);
String receivedData = "none";
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
  for (int i = 0;i<10000;i++) {
    LoRaReceiveAndTampilkanLCD();
  }
  dataSensor = "";
  dataSensor += dht.readTemperature();
  dataSensor += "|";
  dataSensor += dht.readHumidity();
  LoRaSend(dataSensor);
delay(1000);
}




void LoRaReceiveAndTampilkanLCD() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("\nReceived packet '");

    while (LoRa.available()) {
      receivedData = (char)LoRa.read();
      Serial.print(receivedData);
      //Serial.print((char)LoRa.read());
    }
    /*delay(2000);
    lcd.setCursor(0,0);
    lcd.print(receivedData); 
    delay(2000);*/
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}

int LoRaSend(String data) {
  Serial.print("Sending packet: ");
  LoRa.beginPacket();
  LoRa.print("Data: ");
  LoRa.print(data);
  LoRa.endPacket();
}

