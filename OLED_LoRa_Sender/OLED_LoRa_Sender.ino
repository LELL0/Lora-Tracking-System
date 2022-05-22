#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "SSD1306.h"
#include "images.h"

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND  868E6
#define ledpin 25
#define RXPin 22
#define TXPin 23

TinyGPSPlus gps;
SoftwareSerial SerialGPS(RXPin, TXPin);


int sf, cr;

unsigned int counter = 0;

SSD1306 display(0x3c, 21, 22);
String packSize = "--";
String packet ;



void disp(String s1, String s2, String s3) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);

  display.drawStringMaxWidth(0 , 26 , 128, packet);
  display.drawString(0, 0, s1);
  display.drawString(0, 25, s2);
  display.drawString(0, 50, s3);
  display.display();
}

void sendpacket(String packet) {
  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();
}

String data()
{
  String data = "";
  data += counter;

  if (gps.location.isValid()) {
    // data += "Latitude: " + String(gps.location.lat(), 6) + "Longitude: " + String(gps.location.lng(), 6) + "Altitude: " + String(gps.altitude.meters());
    data += "c" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6) + "," + String(gps.altitude.meters());
  }
  else
  {
    data += ",Location";
  }

  //  if (gps.date.isValid()){
  //    data += "," + String(gps.date.month()) + "/" + String(gps.date.day()) + "/" + String(gps.date.year());
  //  } else
  //  {
  //    data += ",Date";
  //  }
  //  if (gps.time.isValid()){
  //    data += ",";
  //    if (gps.time.hour() < 10) Serial.print(F("0"));
  //    data += String(gps.time.hour());
  //    data += ":";
  //    if (gps.time.minute() < 10) Serial.print(F("0"));
  //    data += String(gps.time.minute());
  //    data += ":";
  //    if (gps.time.second() < 10) Serial.print(F("0"));
  //    data += String(gps.time.second());
  //  }
  //  else{
  //    data += String(",Time");
  //  }
  return data;
}

void setup() {
  SerialGPS.begin(9600);

  pinMode(16, OUTPUT);
  pinMode(ledpin, OUTPUT);

  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50);
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high

  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Sender Test");

  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Sta rting LoRa failed!");
    while (1);
  }
  //LoRa.onReceive(cbk);
  //  LoRa.receive();
  Serial.println("init ok");
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  disp("Presented To You", "By:", "Elia El Khoury");
  delay(1500);

  sf = 10;
  cr = 7;
  LoRa.setSpreadingFactor(sf);
  LoRa.setCodingRate4(float(cr));

  sendpacket("Counter,Latitude,Longitude,Altitude,SF,CR");
}


void loop() {

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("GPS NOT DETECTED!");
  }
  else if (!SerialGPS.available() > 0) {
    Serial.println("NO GPS LOCATION");
  }


  String dt = data();

  // Setup Spreading Factor (6 ~ 12)
  // Setup Coding Rate:5(4/5),6(4/6),7(4/7),8(4/8)
  //  if (counter == 200) {
  //    sf = 12;
  //    cr = 8;
  //    LoRa.setSpreadingFactor(sf);
  //    LoRa.setCodingRate4(cr);
  //  }
  //  else if (counter == 400) {
  //    sf = 6;
  //    cr = 9;
  //    LoRa.setSpreadingFactor(sf);
  //    LoRa.setCodingRate4(cr);
  //  } else if (counter == 600) {
  //    sf = 6;
  //    cr = 7;
  //    LoRa.setSpreadingFactor(sf);
  //    LoRa.setCodingRate4(cr);
  //  } else if (counter == 800) {
  //    sf = 12;
  //    cr = 5;
  //    LoRa.setSpreadingFactor(sf);
  //    LoRa.setCodingRate4(cr);
  //  } else if (counter > 1000) {
  //    disp("STOPPED The Experiment", "Counter: " + String(counter) + "Reached", "Thanks...");
  //    exit(0);
  //  }


  if (counter > 1000) {
    disp("STOPPED The Experiment", "Counter: " + String(counter) + "Reached", "Thanks...");
    delay(1000000);
  }

  disp("Counter: " + String(counter), dt, "SF: " + String(sf) + " | CR: " + String(cr));

  String moreDt = "," + String(sf) + "," + String(cr);
  sendpacket(dt + moreDt);
  counter++;
  digitalWrite(ledpin, HIGH);
  delay(100);
  digitalWrite(ledpin, LOW);
  delay(100);
}
