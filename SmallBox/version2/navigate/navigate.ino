// Fixed Constants
// Motor Output Pins
int ENA = 10; // Right Wheel Speed
int IN1 = 9;
int IN2 = 8;
int IN3 = 7;
int IN4 = 6;
int ENB = 5; // Left Wheel Speed

int MOTOR_PINS[6] = {ENA, IN1, IN2, IN3, IN4, ENB};

// Encoder Input Pins
int RIGHT_ENC_YELLOW = 3;
int RIGHT_ENC_GREEN = 12;
int LEFT_ENC_YELLOW = 4;
int LEFT_ENC_GREEN = 2;

// Bumper Input Pins
int RIGHT_BUMP = A1;
int CENTER_BUMP = A2;
int LEFT_BUMP = A3;

// Gloval Variables

int LEFT_WHEEL_COUNT = 0;
int RIGHT_WHEEL_COUNT = 0;

float AVG_WHEEL_SPEED = 0.25;

enum ACTIONS
{
  TURN_LEFT,
  TURN_RIGHT,
  FORWARD,
  BACKWARDS
};

const int ACTIONS_LOG_LENGTH = 100;
int ACTIONS_LOG_INDEX = 0;
enum ACTIONS actionsLog[ACTIONS_LOG_LENGTH];

enum OBSTACLES
{
  LEFT,
  RIGHT,
  CENTER,
  STUCK,
  NONE
};
int LAST_STUCK = 0;

void setupPins()
{

  // Setup Motor Pins
  for (int i = 0; i < 6; i++)
  {
    pinMode(MOTOR_PINS[i], OUTPUT);
  }

  // Setup Encoder Pins
  pinMode(RIGHT_ENC_YELLOW, INPUT);
  pinMode(RIGHT_ENC_GREEN, INPUT);
  pinMode(LEFT_ENC_YELLOW, INPUT);
  pinMode(LEFT_ENC_GREEN, INPUT);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_YELLOW), rightWheelPulse, RISING);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_GREEN), leftWheelPulse, RISING);

  // Setup Bumper Pins
  pinMode(RIGHT_BUMP, INPUT);
  pinMode(CENTER_BUMP, INPUT);
  pinMode(LEFT_BUMP, INPUT);
}

// Speed Control
void rightWheelPulse()
{
  int rightGreen = digitalRead(RIGHT_ENC_GREEN);
  if (rightGreen == 1)
  {
    RIGHT_WHEEL_COUNT++;
  }
  else
  {
    RIGHT_WHEEL_COUNT++;
  }
}

void leftWheelPulse()
{
  int leftYellow = digitalRead(LEFT_ENC_YELLOW);
  if (leftYellow == 1)
  {
    LEFT_WHEEL_COUNT++;
  }
  else
  {
    LEFT_WHEEL_COUNT++;
  }
}

// Motor Controls

void setWheelSpeed(float motorSpeed)
{

  int PWM = (int)(motorSpeed * 255);
  analogWrite(ENA, PWM);
  analogWrite(ENB, PWM);
}

void forward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void backward()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Bumper Sensing
int getBump()
{
  int left = analogRead(LEFT_BUMP) > 400;
  int center = analogRead(CENTER_BUMP) > 400;
  int right = analogRead(RIGHT_BUMP) > 400;
  int total = (left << 2) | (center << 1) | right;
  Serial.print("Bumper Readings: ");
  Serial.print(left);
  Serial.print(" ");
  Serial.print(center);
  Serial.print(" ");
  Serial.println(right);
  return total;
}

// Decide Move
void makeMove(enum OBSTACLES obstacle)
{
  LAST_STUCK = 0;
  switch (obstacle)
  {
  case NONE:
    forward();
    break;
  case LEFT:
    turnRight();
    actionsLog[ACTIONS_LOG_INDEX] = TURN_RIGHT;
    ACTIONS_LOG_INDEX = (ACTIONS_LOG_INDEX + 1) % ACTIONS_LOG_LENGTH;
    break;
  case RIGHT:
    turnLeft();
    actionsLog[ACTIONS_LOG_INDEX] = TURN_LEFT;
    ACTIONS_LOG_INDEX = (ACTIONS_LOG_INDEX + 1) % ACTIONS_LOG_LENGTH;
    break;
  case CENTER:
    handleCenter();
    break;
  case STUCK:
    handleStuck();
    LAST_STUCK = 1;
    break;
  }
}

void handleCenter()
{
  if (actionsLog[ACTIONS_LOG_INDEX - 1] == TURN_LEFT)
  {
    turnLeft();
    actionsLog[ACTIONS_LOG_INDEX] = TURN_LEFT;
    ACTIONS_LOG_INDEX = (ACTIONS_LOG_INDEX + 1) % ACTIONS_LOG_LENGTH;
  }
  else
  {
    turnRight();
    actionsLog[ACTIONS_LOG_INDEX] = TURN_RIGHT;
    ACTIONS_LOG_INDEX = (ACTIONS_LOG_INDEX + 1) % ACTIONS_LOG_LENGTH;
  }
}

void handleStuck()
{
  backward();
  delay(500);
  if (actionsLog[ACTIONS_LOG_INDEX - 1] == TURN_LEFT)
  {
    turnLeft();
    actionsLog[ACTIONS_LOG_INDEX] = TURN_LEFT;
    ACTIONS_LOG_INDEX = (ACTIONS_LOG_INDEX + 1) % ACTIONS_LOG_LENGTH;
  }
  else
  {
    turnRight();
    actionsLog[ACTIONS_LOG_INDEX] = TURN_RIGHT;
    ACTIONS_LOG_INDEX = (ACTIONS_LOG_INDEX + 1) % ACTIONS_LOG_LENGTH;
  }
}

void setup()
{
  Serial.begin(115200);
  setupPins();
  setWheelSpeed(AVG_WHEEL_SPEED);
  forward();
  delay(300);
}

void loop()
{
  setWheelSpeed(AVG_WHEEL_SPEED);
  int bumperValue = getBump();
  enum OBSTACLES obstacle = NONE;
  // 7: 111, 6: 110, 5: 101, 4: 100, 3: 011, 2: 010, 1: 001, 0: 000
  if (bumperValue == 7 || bumperValue == 5 || bumperValue == 2)
  {
    obstacle = CENTER;
  }
  else if (bumperValue == 6 || bumperValue == 4)
  {
    obstacle = LEFT;
  }
  else if (bumperValue == 3 || bumperValue == 1)
  {
    obstacle = RIGHT;
  }

  if (!LAST_STUCK && abs(LEFT_WHEEL_COUNT) < 30 && abs(RIGHT_WHEEL_COUNT) < 30)
  {
    obstacle = STUCK;
  }

  if (!LAST_STUCK && (abs(LEFT_WHEEL_COUNT) < 5 || abs(RIGHT_WHEEL_COUNT) < 5))
  {
    obstacle = STUCK;
  }
  makeMove(obstacle);
  Serial.print("Encoder Readings: ");
  Serial.print(LEFT_WHEEL_COUNT);
  Serial.print(" ");
  Serial.println(RIGHT_WHEEL_COUNT);
  LEFT_WHEEL_COUNT = RIGHT_WHEEL_COUNT = 0;
  delay(200);
}
