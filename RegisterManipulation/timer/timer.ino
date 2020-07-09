void setup()
{
    Serial.begin(115200);
    portManipulation();
    timerManipulation();
}

void loop()
{
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