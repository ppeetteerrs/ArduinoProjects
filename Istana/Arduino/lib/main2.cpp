// Import Arduino libraries and pre-defined functions
#include "Arduino.h"

// Define metadata
#define N_INPUTS 3
#define N_SAMPLES_VALUES 50
#define N_SAMPLES_VARIANCE 50

// Preset variables
int pitches[9] = {0, 880, 1047, 1319, 1568, 1976, 2349, 2794, 3520};
uint8_t input_pins[N_INPUTS] = {A5, A6, A7};

// Buffer Variables
int raw_values[N_INPUTS][N_SAMPLES_VALUES];
int variances[N_INPUTS][N_SAMPLES_VARIANCE];

// Tracking variables
int raw_value_index = 0;
int variance_index = 0;

double movingAverage()
{
  double sum = 0;
  for (int i = 0; i < N_SAMPLES; i++)
  {
    sum += values[i];
  }
  return sum / N_SAMPLES;
}

double movingStdDev()
{
  double sum = 0;
  double variance = 0;
  for (int i = 0; i < N_SAMPLES; i++)
  {
    sum += values[i];
  }
  double avg = sum / N_SAMPLES;
  for (int i = 0; i < N_SAMPLES; i++)
  {
    variance += (values[i] - avg) * (values[i] - avg);
  }
  return variance / N_SAMPLES;
}

void setup()
{
  // Setup serial communication
  Serial.begin(115200);

  // Setup pinModes
  for (uint8_t pin : input_pins)
  {
    pinMode(pin, INPUT);
  }

  // Initialize zero arrays
  for (int &value : raw_values)
  {
    value = 0;
  }
  for (int &variance : variances)
  {
    variance = 0;
  }
}

void loop()
{

  // Read pitch
  for (uint8_t pin : input_pins)
  {
    int pitch = analogRead(pin);
  }

  values[currentIndex] = pitch;
  currentIndex = (currentIndex + 1) % N_SAMPLES;

  int _average = (int)(movingAverage() / 1023 * 8);
  int _variance = (int)(movingStdDev());

  tone(6, pitches[8 - _average]);

  //Serial.println(pitches[8-filtered]);
  //    Serial.println(pitch);
  Serial.println(_variance);
  delay(10);
}