int LED = A0;
int i = 0;

void setup()
{
    Serial.begin(57600);
    pinMode(LED, INPUT);
}

void loop()
{
    int reading = analogRead(LED);
    Serial.print(i);
    Serial.print(": ");
    Serial.println(reading);
    delay(500);
    i++;
}