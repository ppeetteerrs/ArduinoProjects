double R0 = 1000;
double R1 = 983;
double a = 3.90802E-3;
double b = 5.80195E-7;

void setup()
{
    // initialize the serial communication:
    Serial.begin(9600);
    pinMode(A4, INPUT); // Setup for leads off detection LO +
    portManipulation();
    timerManipulation();
}

void loop()
{
    int m = (double)analogRead(A4);
    double Rt = m * R1 / (1024 - m);
    double t = (sqrt(a * a - 4 * b * (1 - Rt / R0)) - a) / (2 * b);

    Serial.print("Analog Reading: ");
    Serial.print(m);
    Serial.print(", Resistance: ");
    Serial.print(Rt);
    Serial.print(", Temperature: ");
    Serial.println(t);
    delay(1000);
}

void portManipulation()
{
    // Set Pins 7:0 to OUTPUT mode
    DDRD = 0b11111111;
    // Set Pins 13:8 to OUTPUT mode
    DDRB = (DDRB & 0b00111111) | 0b00111111;
    // SET Pin 3 and 11 to HIGH for PWM
    // SET Pin 12 and 13 to HIGH for Direction
    PORTD = (PORTD & 0b00001000) | 0b00001000;
    PORTB = (PORTB & 0b00111000) | 0b00111000;
}

void timerManipulation()
{
    TCCR1A = 0x00;
    TCCR1B = (TCCR1B & 0b11100000) | 0b01010;

    // Set to CTC Mode
    TIMSK1 = (TIMSK1 & 0b11111000) | 0b010;

    OCR1A = 18;
}

ISR(TIMER1_COMPA_vect)
{
    // Flip Pins 12 and 13
    PORTB ^= 0b00110000;
}
