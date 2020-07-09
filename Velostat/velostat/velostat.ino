int prevValue = 0;
int count = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(A0, INPUT);
}

void loop()
{
    int value = analogRead(A0);
    if (abs(value - prevValue) > 10)
    {
        count++;
        Serial.print("Touched (");
        Serial.print(value);
        Serial.print(", ");
        Serial.print(prevValue);
        Serial.print(") ");
        Serial.println(count);
    }
    prevValue = value;
    delay(200);
}