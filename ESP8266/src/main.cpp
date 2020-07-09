#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

/* Put your SSID & Password */
const char *ssid = "NodeMCU";      // Enter SSID here
const char *password = "12345678"; //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

Servo servo;

uint8_t ServoPin = D6;
uint8_t LEDPin = D4;
int ServoZero = 0;
int ServoFull = 180;

bool needToOpen = false;

String SendHTML()
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-on:active {background-color: #16a085;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Garage Secret Access</h1>\n";
  ptr += "<p>Only for naughty people =(</p>\n";
  ptr += "<a class=\"button button-on\" href=\"/open\">OPEN</a>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void openDoor()
{
}

void handleRoot()
{
  needToOpen = false;
  server.send(200, "text/html", SendHTML());
}

void handleOpen()
{
  needToOpen = true;
  Serial.println("Opening Door...");
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleNotFound()
{
  server.send(404, "text/plain", "Not found");
}

void setup()
{
  Serial.begin(115200);
  servo.attach(ServoPin);
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  delay(100);

  server.on("/", handleRoot);
  server.on("/open", handleOpen);
  server.onNotFound(handleNotFound);

  server.begin();
  digitalWrite(LEDPin, HIGH);
  Serial.println("HTTP server started");
  servo.write(ServoZero);
}
void loop()
{
  server.handleClient();
  if (needToOpen)
  {
    servo.write(ServoZero);
    delay(500);
    servo.write(ServoFull);
    delay(2000);
    servo.write(ServoZero);
    needToOpen = false;
  }
  else
  {
    servo.write(ServoZero);
  }
}
