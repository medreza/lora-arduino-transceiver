#include <SPI.h>
#include <LoRa.h>
#include <LiquidCrystal.h>
#include <DHT.h>


#define DHTPIN A1 //DHT pin A1
#define DHTTYPE DHT11
LiquidCrystal lcd(8, 3, 4, 5, 6, 7);
DHT dht(DHTPIN, DHTTYPE);

void LoRaReceiveAndTampilkanLCD();
int LoRaSend(String dataSensor);
String receivedData = "";
String receivedDataTemp = "";
String temp = "";
String dataSensor;
int totData = 0;
int tempTotTemp = 0;
int tempTotHum = 0;

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
  Serial.println("\nMode Receiver");
  lcd.setCursor(15,0);
  lcd.print("R");
  for (int i = 0;i<20000;i++) {
    LoRaReceiveAndTampilkanLCD();
  }
  dataSensor += "T";
  dataSensor += int(dht.readTemperature());
  dataSensor += " H";
  dataSensor += int(dht.readHumidity());
  Serial.println("\nMode Sender");
  lcd.setCursor(15,0);
  lcd.print("S");
  LoRaSend(dataSensor);
  lcd.setCursor(0,0);
  lcd.print(dataSensor); 
  lcd.setCursor(0,1);
  lcd.print(receivedData);
  delay(1000);
  LoRaSend(dataSensor);
  delay(1000);
  LoRaSend(dataSensor);
}


void LoRaReceiveAndTampilkanLCD() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet '");

    while (LoRa.available()) {
      temp = (char)LoRa.read();
      Serial.print(temp);
      receivedDataTemp += temp;
      //Serial.print((char)LoRa.read());
    }
    receivedData = receivedDataTemp ;
    receivedDataTemp = "";
    tempTotTemp += (receivedData.substring(1,3)).toInt();
    tempTotHum += (receivedData.substring(5)).toInt();
    totData++;
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    lcd.setCursor(8,0);
    lcd.print(LoRa.packetRssi());
    lcd.setCursor(11,0);
    lcd.print("dB");
    lcd.setCursor(8,1);
    lcd.print("rT");
    lcd.setCursor(10,1);
    lcd.print(tempTotTemp/totData);
    lcd.setCursor(12,1);
    lcd.print("rH");
    lcd.setCursor(14,1);
    lcd.print(tempTotHum/totData);
  }
}

int LoRaSend(String data) {
  Serial.print("Sending packet: ");
  LoRa.beginPacket();
  LoRa.print(data);
  LoRa.endPacket();
  Serial.println(data);
}


