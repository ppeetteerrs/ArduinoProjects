#include <Servo.h>
#define MODE 1                    // 0 for auto motion dection, 1 for joystick aiming
#define JOYSTICK_UPDATE_PERIOD 20 // Update servo every how many milliseconds
#define SERVO_RESOLUTION 1        // Angle to increase each time servo is updated
#define SHOOT_PERIOD 400

// Servo Info
Servo bottomServo;
Servo shootServo;
Servo armServo;
int bottomServoPin = 9;
int shootServoPin = 10;
int armServoPin = 11;
int bottomServoAngle = 90;
int shootServoAngle = 0;
int armServoAngle = 90;

int xPin = A7;
int yPin = A6;
int swPin = A5;

// For Serial Print Purposes
struct Inputs
{
    int x;
    int y;
    unsigned long lastPrinted;
} inputs;

// For Serial Print Purposes
struct JoystickInfo
{
    int x;
    int y;
    int shoot;
    unsigned long lastShot;
    unsigned long lastUpdate;
    bool reload;
} jsInfo;

void setup()
{
    Serial.begin(115200);

    // Pins setup
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);
    pinMode(swPin, INPUT_PULLUP);

    // Servos setup
    bottomServo.attach(bottomServoPin);
    shootServo.attach(shootServoPin);
    armServo.attach(armServoPin);
    // bottomServo.write(90); // Middle
    // shootServo.write(0);   // Ready to fire
    // armServo.write(90);    // Middle

    // Ready
    Serial.println("Ready");
}

void loop()
{
    readInputs();
    if (coolDown(&(jsInfo.lastUpdate), JOYSTICK_UPDATE_PERIOD))
    {
        if (jsInfo.shoot && coolDown(&(jsInfo.lastShot), SHOOT_PERIOD))
        {
            shoot(); // Shoot if asked to
        }
        else
        {
            calculateAngles(); // Update servo angles if not shooting
        }
        if (coolDown(&(inputs.lastPrinted), SHOOT_PERIOD))
        {
            printInfo();
        }
        updateServos();
    }
}

bool coolDown(unsigned long *lastTime, int period)
{
    unsigned long currentTime = millis();
    if (currentTime - *lastTime > period)
    {
        *lastTime = currentTime;
        return 1;
    }
    else
    {
        return 0;
    }
}

void readInputs()
{
    inputs.x = analogRead(xPin);
    inputs.y = analogRead(yPin);
    jsInfo.shoot = digitalRead(swPin) == 0;
    jsInfo.x = inputs.x < 300 ? -1 : inputs.x > 700 ? 1 : 0;
    jsInfo.y = inputs.y < 300 ? -1 : inputs.y > 700 ? 1 : 0;
}

void calculateAngles()
{
    armServoAngle = limit(armServoAngle + jsInfo.x * SERVO_RESOLUTION);
    bottomServoAngle = limit(bottomServoAngle + jsInfo.y * SERVO_RESOLUTION);
}

int limit(int angle)
{
    return angle > 180 ? 180 : angle < 0 ? 0 : angle;
}

void shoot()
{
    if (shootServoAngle <= 120)
    {
        shootServoAngle += 60;
    }
}

void updateServos()
{
    bottomServo.write(bottomServoAngle);
    armServo.write(armServoAngle);
    shootServo.write(shootServoAngle);
    if (shootServoAngle >= 180)
    {
        shootServoAngle = 0;
        delay(500);
        shootServo.write(shootServoAngle);
    }

    jsInfo.shoot = 0;
    jsInfo.x = 0;
    jsInfo.y = 0;
}

void printInfo()
{
    Serial.print("Inputs:");
    // Serial.print(activated);
    Serial.print(" X - ");
    Serial.print(inputs.x);
    Serial.print(" Y - ");
    Serial.print(inputs.y);
    Serial.print(" Shoot - ");
    Serial.println(jsInfo.shoot);

    Serial.print("Angles:");
    Serial.print(" Bottom - ");
    Serial.print(bottomServoAngle);
    Serial.print(" Arm - ");
    Serial.print(armServoAngle);
    Serial.print(" Shoot - ");
    Serial.println(shootServoAngle);
    Serial.println();
    // Serial.println(servoAngle);
}