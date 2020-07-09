// FAN
const int fanSpeedPin = 11;
const int FANSPEED = 200;

// ULTRASONIC
const int trigPin = 2;
const int cliffEchoPin = 19;
const int rightEchoPin = 17;
const int leftEchoPin = 18;

// DISTANCE SENSOR
const int sensorPin = 14;
const int sensorTrigPin = 4;

// MOTORS: 0 is right, 1 is left
const int motorPins[2][3] = {{5, 6, 7}, {10, 8, 9}};
const int MOTORSPEED = 120;
const int RIGHTSPEEDBOOST = 5;
const double TURN_THRESHOLD = 8.0;
const double TURN_ANGLED_THRESHOLD = 50.0;
const int STUCK_THRESHOLD = 5;
const int STUCK_REST = 230;

//PROGRAM
int stuckCount = 0;
double distanceCache[3] = { -1.0, -1.0, -1.0};

void setup() {
  pinMode(fanSpeedPin, OUTPUT);
  setFanSpeed(FANSPEED);

  pinMode(trigPin, OUTPUT);
  pinMode(cliffEchoPin, INPUT);
  pinMode(rightEchoPin, INPUT);
  pinMode(leftEchoPin, INPUT);
  
  pinMode(sensorTrigPin, OUTPUT);
  pinMode(sensorPin, INPUT);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      pinMode(motorPins[i][j], OUTPUT);
    }
  }

  Serial.begin(9600);
}

void loop() {
  delay(50);
  double leftDistance = getDistance(leftEchoPin);
  double rightDistance = getDistance(rightEchoPin);
  double cliffDistance = getDistance(cliffEchoPin);
  double sensorDistance = getSensorDistance();
  Serial.print("Distances are (left, right, cliff) ");
  Serial.print(leftDistance);
  Serial.print("cm ");
  Serial.print(rightDistance);
  Serial.print("cm ");
  Serial.print(cliffDistance);
  Serial.println("cm.");
  Serial.print("Sensor distance is: ");
  Serial.print(sensorDistance);
  Serial.println("cm.");
  if (stuckCount >= STUCK_THRESHOLD || (distanceCache[2] > 0 && distanceCache[2] < cliffDistance - 0.5)) {
    stuckCount = 0;
    moveBackwards(MOTORSPEED + 10);
    delay(500);
  } else if (isStuck(leftDistance, rightDistance, sensorDistance) == 1) {
    stuckCount++;
    turnLeft(MOTORSPEED);
    delay(STUCK_REST);
    distanceCache[2] = cliffDistance;
  } else if (isStuck(leftDistance, rightDistance, sensorDistance) == -1) {
    stuckCount++;
    turnRight(MOTORSPEED);
    delay(STUCK_REST);
    distanceCache[2] = cliffDistance;
  } else {
    stuckCount = 0;
    moveForward(MOTORSPEED);
    distanceCache[2] = cliffDistance;
  }
  distanceCache[0] = leftDistance;
  distanceCache[1] = rightDistance;
}

double getDistance(int pin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(20);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  long duration = pulseIn(pin, HIGH);
  double distance = duration * 0.034 / 2;
  if (distance < 0.1) {
    distance = 300.0;
  }
  delay(20);
  return distance;
}

double getSensorDistance() {
  digitalWrite(sensorTrigPin, HIGH);
  delay(20);
  int sensorVal = analogRead(sensorPin);
  if (sensorVal >= 80 && sensorVal <= 490) {
    return 9462 / ((double) sensorVal - 16.92);
  } else {
    return 100;
  }
  delay(10);
  digitalWrite(sensorTrigPin, LOW);
}

void setFanSpeed(int speed) {
  analogWrite(fanSpeedPin, speed);
}

void wheelMove(int wheel, int speed) {
  if (wheel == 0) {
    speed += RIGHTSPEEDBOOST;
  } else {
    speed -= RIGHTSPEEDBOOST;
  }
  if (speed >= 0) {
    analogWrite(motorPins[wheel][0], speed);
    digitalWrite(motorPins[wheel][1], HIGH);
    digitalWrite(motorPins[wheel][2], LOW);
  } else {
    analogWrite(motorPins[wheel][0], -speed);
    digitalWrite(motorPins[wheel][1], LOW);
    digitalWrite(motorPins[wheel][2], HIGH);
  }
}

void moveForward(int speed) {
  wheelMove(0, speed);
  wheelMove(1, speed);
}

void moveBackwards(int speed) {
  wheelMove(0, -speed);
  wheelMove(1, -speed);
}

void turnLeft(int speed) {
  wheelMove(0, speed);
  wheelMove(1, -speed);
}

void turnRight(int speed) {
  wheelMove(0, -speed);
  wheelMove(1, speed);
}

/*
  int isStuck(double leftDistance, double rightDistance) {
    if (leftDistance >= rightDistance && (rightDistance < TURN_THRESHOLD || (pow(rightDistance, 1.5) / leftDistance < 1.5))) {
    return 1;
  } else if (rightDistance >= leftDistance && (leftDistance < TURN_THRESHOLD || (pow(leftDistance, 1.5) / rightDistance < 1.5))) {
    return -1;
  } else {
    return 0;
  }
  }
*/


int isStuck(double leftDistance, double rightDistance, double sensorDistance) {

  // Just started
  if (distanceCache[0] < 0 || distanceCache[1] < 0) {
    return 0;
  } else if (leftDistance >= rightDistance) {
    // Right side stuck
    if (rightDistance < TURN_THRESHOLD || sensorDistance < 30.0) {
      // Too close
      return 1;
    } else if (sensorDistance < 50.0 && rightDistance > 1000.0) {
      // Blocked
      return 1;
    } else if (sensorDistance <350.0 && fabs(rightDistance - distanceCache[1]) < 1.0) {
      // Did not move
      return 1;
    } else {
      return 0;
    }
  } else if (rightDistance >= leftDistance) {
    // Left side stuck
    if (leftDistance < TURN_THRESHOLD || sensorDistance < 30.0) {
      // Too close
      return -1;
    } else if (sensorDistance < 50.0 && leftDistance > 1000.0) {
      // Blocked
      return -1;
    } else if (sensorDistance < 35.0 && fabs(leftDistance - distanceCache[0]) < 1.0) {
      // Did not move
      return -1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}
