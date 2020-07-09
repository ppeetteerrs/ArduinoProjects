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
void updateDisplay(char *message);

void setup() {
    Wire.begin();
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  Serial.println("Listening");
}
void loop() {
  if (radio.available()) {
    Serial.print("Incoming: ");
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    updateDisplay(text);
  }
}

void updateDisplay(char *message) {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print(message);
    display.display();
}
