#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>

#include "SSD1306.h"
#include "images.h"

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    868E6
#define ledpin 25

unsigned int counter = 0;

int sf, cr;

SSD1306 display(0x3c, 21, 22);
String rssi = "RSSI --";
String packSize = "--";
String packet ;


void loraData() {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0 , 15 , "Received " + packSize + " bytes");
  display.drawStringMaxWidth(0 , 26 , 128, packet);
  display.drawString(0, 0, "RSSI --" + rssi);
  display.drawString(0, 50, "Packets recieved: " + String(counter));

  display.display();
  Serial.println(rssi + "," + packet);
}

void disp(String s1, String s2, String s3) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);

  display.drawStringMaxWidth(0 , 26 , 128, packet);
  display.drawString(0, 0, s1);
  display.drawString(0 , 20 , s2);
  display.drawString(0, 40, s3);

  display.display();
  Serial.println(rssi);
}


void cbk(int packetSize) {
  packet = "";
  packSize = String(packetSize, DEC);
  //  Serial.println("packSize: " + packSize);
  for (int i = 0; i < packetSize; i++) {
    packet += (char) LoRa.read();
  }
  rssi = String(LoRa.packetRssi(), DEC) ;
  loraData();
}

void setup() {
  pinMode(16, OUTPUT);
  pinMode(ledpin, OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50);
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high、

  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Receiver Callback");
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //LoRa.onReceive(cbk);
  LoRa.receive();
  Serial.println("init ok");
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  disp("Presented To you", "By", "Elia El Khoury");

  sf = 10;
  cr = 7;
  LoRa.setSpreadingFactor(sf);
  LoRa.setCodingRate4(cr);

  digitalWrite(ledpin, HIGH);

  delay(1500);
}

void loop() {

  digitalWrite(ledpin, LOW);
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0) {
    cbk(packetSize);
    counter++;
    digitalWrite(ledpin, HIGH);

    delay(10);
  }
}
