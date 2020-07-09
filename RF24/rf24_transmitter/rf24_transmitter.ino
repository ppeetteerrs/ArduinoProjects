#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
void setup() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.println("Started");
}
void loop() {
  const char text[] = "Hello World";
  radio.write(text, sizeof(text));
  Serial.print("Sent: ");
  Serial.println(text);
  delay(500);
}
